/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2010, 2011, 2012 Apple Inc. All rights reserved.
 *
 * Portions are Copyright (C) 1998 Netscape Communications Corporation.
 *
 * Other contributors:
 *   Robert O'Callahan <roc+@cs.cmu.edu>
 *   David Baron <dbaron@fas.harvard.edu>
 *   Christian Biesinger <cbiesinger@web.de>
 *   Randall Jesup <rjesup@wgate.com>
 *   Roland Mainz <roland.mainz@informatik.med.uni-giessen.de>
 *   Josh Soref <timeless@mac.com>
 *   Boris Zbarsky <bzbarsky@mit.edu>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Alternatively, the contents of this file may be used under the terms
 * of either the Mozilla Public License Version 1.1, found at
 * http://www.mozilla.org/MPL/ (the "MPL") or the GNU General Public
 * License Version 2.0, found at http://www.fsf.org/copyleft/gpl.html
 * (the "GPL"), in which case the provisions of the MPL or the GPL are
 * applicable instead of those above.  If you wish to allow use of your
 * version of this file only under the terms of one of those two
 * licenses (the MPL or the GPL) and not to allow others to use your
 * version of this file under the LGPL, indicate your decision by
 * deletingthe provisions above and replace them with the notice and
 * other provisions required by the MPL or the GPL, as the case may be.
 * If you do not delete the provisions above, a recipient may use your
 * version of this file under any of the LGPL, the MPL or the GPL.
 */

#include "config.h"
#include "core/rendering/RenderLayerClipper.h"

#include "core/rendering/RenderLayer.h"
#include "core/rendering/RenderView.h"

namespace WebCore {

void RenderLayerClipper::updateClipRects(const ClipRectsContext& clipRectsContext)
{
    ClipRectsType clipRectsType = clipRectsContext.clipRectsType;
    ASSERT(clipRectsType < NumCachedClipRectsTypes);
    if (m_clipRectsCache && m_clipRectsCache->getClipRects(clipRectsType, clipRectsContext.respectOverflowClip)) {
        ASSERT(clipRectsContext.rootLayer == m_clipRectsCache->m_clipRectsRoot[clipRectsType]);
        ASSERT(m_clipRectsCache->m_scrollbarRelevancy[clipRectsType] == clipRectsContext.overlayScrollbarSizeRelevancy);

#ifdef CHECK_CACHED_CLIP_RECTS
        // This code is useful to check cached clip rects, but is too expensive to leave enabled in debug builds by default.
        ClipRectsContext tempContext(clipRectsContext);
        tempContext.clipRectsType = TemporaryClipRects;
        ClipRects clipRects;
        calculateClipRects(tempContext, clipRects);
        ASSERT(clipRects == *m_clipRectsCache->getClipRects(clipRectsType, clipRectsContext.respectOverflowClip).get());
#endif
        return; // We have the correct cached value.
    }

    // For transformed layers, the root layer was shifted to be us, so there is no need to
    // examine the parent. We want to cache clip rects with us as the root.
    RenderLayer* parentLayer = clipRectsContext.rootLayer != m_renderer->layer() ? m_renderer->layer()->parent() : 0;
    if (parentLayer)
        parentLayer->clipper().updateClipRects(clipRectsContext);

    ClipRects clipRects;
    calculateClipRects(clipRectsContext, clipRects);

    if (!m_clipRectsCache)
        m_clipRectsCache = adoptPtr(new ClipRectsCache);

    if (parentLayer && parentLayer->clipper().clipRects(clipRectsContext) && clipRects == *parentLayer->clipper().clipRects(clipRectsContext))
        m_clipRectsCache->setClipRects(clipRectsType, clipRectsContext.respectOverflowClip, parentLayer->clipper().clipRects(clipRectsContext));
    else
        m_clipRectsCache->setClipRects(clipRectsType, clipRectsContext.respectOverflowClip, ClipRects::create(clipRects));

#ifndef NDEBUG
    m_clipRectsCache->m_clipRectsRoot[clipRectsType] = clipRectsContext.rootLayer;
    m_clipRectsCache->m_scrollbarRelevancy[clipRectsType] = clipRectsContext.overlayScrollbarSizeRelevancy;
#endif
}

void RenderLayerClipper::clearClipRectsIncludingDescendants(ClipRectsType typeToClear)
{
    // FIXME: it's not clear how this layer not having clip rects guarantees that no descendants have any.
    if (!m_clipRectsCache)
        return;

    clearClipRects(typeToClear);

    for (RenderLayer* layer = m_renderer->layer()->firstChild(); layer; layer = layer->nextSibling())
        layer->clipper().clearClipRectsIncludingDescendants(typeToClear);
}

void RenderLayerClipper::clearClipRects(ClipRectsType typeToClear)
{
    if (typeToClear == AllClipRectTypes) {
        m_clipRectsCache = nullptr;
    } else {
        ASSERT(typeToClear < NumCachedClipRectsTypes);
        RefPtr<ClipRects> dummy;
        m_clipRectsCache->setClipRects(typeToClear, RespectOverflowClip, dummy);
        m_clipRectsCache->setClipRects(typeToClear, IgnoreOverflowClip, dummy);
    }
}

void RenderLayerClipper::calculateClipRects(const ClipRectsContext& clipRectsContext, ClipRects& clipRects) const
{
    if (!m_renderer->layer()->parent()) {
        // The root layer's clip rect is always infinite.
        clipRects.reset(PaintInfo::infiniteRect());
        return;
    }

    ClipRectsType clipRectsType = clipRectsContext.clipRectsType;
    bool useCached = clipRectsType != TemporaryClipRects;

    // For transformed layers, the root layer was shifted to be us, so there is no need to
    // examine the parent. We want to cache clip rects with us as the root.
    RenderLayer* parentLayer = clipRectsContext.rootLayer != m_renderer->layer() ? m_renderer->layer()->parent() : 0;

    // Ensure that our parent's clip has been calculated so that we can examine the values.
    if (parentLayer) {
        if (useCached && parentLayer->clipper().clipRects(clipRectsContext)) {
            clipRects = *parentLayer->clipper().clipRects(clipRectsContext);
        } else {
            ClipRectsContext parentContext(clipRectsContext);
            parentContext.overlayScrollbarSizeRelevancy = IgnoreOverlayScrollbarSize; // FIXME: why?
            parentLayer->clipper().calculateClipRects(parentContext, clipRects);
        }
    } else {
        clipRects.reset(PaintInfo::infiniteRect());
    }

    // A fixed object is essentially the root of its containing block hierarchy, so when
    // we encounter such an object, we reset our clip rects to the fixedClipRect.
    if (m_renderer->style()->position() == FixedPosition) {
        clipRects.setPosClipRect(clipRects.fixedClipRect());
        clipRects.setOverflowClipRect(clipRects.fixedClipRect());
        clipRects.setFixed(true);
    } else if (m_renderer->style()->hasInFlowPosition()) {
        clipRects.setPosClipRect(clipRects.overflowClipRect());
    } else if (m_renderer->style()->position() == AbsolutePosition) {
        clipRects.setOverflowClipRect(clipRects.posClipRect());
    }

    // Update the clip rects that will be passed to child layers.
    if ((m_renderer->hasOverflowClip() && (clipRectsContext.respectOverflowClip == RespectOverflowClip || m_renderer->layer() != clipRectsContext.rootLayer)) || m_renderer->hasClip()) {
        // This layer establishes a clip of some kind.

        // This offset cannot use convertToLayerCoords, because sometimes our rootLayer may be across
        // some transformed layer boundary, for example, in the RenderLayerCompositor overlapMap, where
        // clipRects are needed in view space.
        LayoutPoint offset;
        offset = roundedLayoutPoint(m_renderer->localToContainerPoint(FloatPoint(), clipRectsContext.rootLayer->renderer()));
        RenderView* view = m_renderer->view();
        ASSERT(view);
        if (view && clipRects.fixed() && clipRectsContext.rootLayer->renderer() == view) {
            offset -= view->frameView()->scrollOffsetForFixedPosition();
        }

        if (m_renderer->hasOverflowClip()) {
            ClipRect newOverflowClip = toRenderBox(m_renderer)->overflowClipRect(offset, clipRectsContext.region, clipRectsContext.overlayScrollbarSizeRelevancy);
            if (m_renderer->style()->hasBorderRadius())
                newOverflowClip.setHasRadius(true);
            clipRects.setOverflowClipRect(intersection(newOverflowClip, clipRects.overflowClipRect()));
            if (m_renderer->isPositioned())
                clipRects.setPosClipRect(intersection(newOverflowClip, clipRects.posClipRect()));
        }
        if (m_renderer->hasClip()) {
            LayoutRect newPosClip = toRenderBox(m_renderer)->clipRect(offset, clipRectsContext.region);
            clipRects.setPosClipRect(intersection(newPosClip, clipRects.posClipRect()));
            clipRects.setOverflowClipRect(intersection(newPosClip, clipRects.overflowClipRect()));
            clipRects.setFixedClipRect(intersection(newPosClip, clipRects.fixedClipRect()));
        }
    }
}

} // namespace WebCore
