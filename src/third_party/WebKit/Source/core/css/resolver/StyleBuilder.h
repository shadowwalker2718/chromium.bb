/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef StyleBuilder_h
#define StyleBuilder_h

#include "CSSPropertyNames.h"
#include "platform/heap/Handle.h"

namespace WebCore {

class CSSValue;
class Document;
class RenderStyle;
class StyleResolverState;

class StyleBuilder {
    STATIC_ONLY(StyleBuilder);
public:
    static void applyProperty(CSSPropertyID, StyleResolverState&, CSSValue*);

    // This function contains the gigantic old switch-statement of properties inherited from
    // StyleResolver. Each property should be migrated over to a new StyleBuilder templated
    // function and removed from this code. Once they're all moved, this function can die.
    static void oldApplyProperty(CSSPropertyID, StyleResolverState&, CSSValue*, bool isInitial, bool isInherit);

private:
    static bool applyProperty(CSSPropertyID, StyleResolverState&, CSSValue*, bool isInitial, bool isInherit);
};

}

#endif
