// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_GFX_RENDER_TEXT_WIN_H_
#define UI_GFX_RENDER_TEXT_WIN_H_
#pragma once

#include <usp10.h>

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "ui/gfx/render_text.h"

namespace gfx {

namespace internal {

struct TextRun {
  TextRun();
  ~TextRun();

  ui::Range range;
  Font font;
  // TODO(msw): Disambiguate color, strike, etc. from TextRuns.
  //            Otherwise, this breaks the glyph shaping process.
  //            See the example at: http://www.catch22.net/tuts/neatpad/12.
  SkColor foreground;
  // A gfx::Font::FontStyle flag to specify bold and italic styles.
  // Supersedes |font.GetFontStyle()|. Stored separately to avoid calling
  // |font.DeriveFont()|, which is expensive on Windows.
  int font_style;
  bool strike;
  bool diagonal_strike;
  bool underline;

  int width;
  // The cumulative widths of preceding runs.
  int preceding_run_widths;

  SCRIPT_ANALYSIS script_analysis;

  scoped_array<WORD> glyphs;
  scoped_array<WORD> logical_clusters;
  scoped_array<SCRIPT_VISATTR> visible_attributes;
  int glyph_count;

  scoped_array<int> advance_widths;
  scoped_array<GOFFSET> offsets;
  ABC abc_widths;
  SCRIPT_CACHE script_cache;

 private:
  DISALLOW_COPY_AND_ASSIGN(TextRun);
};

}  // namespace internal

// RenderTextWin is the Windows implementation of RenderText using Uniscribe.
class RenderTextWin : public RenderText {
 public:
  RenderTextWin();
  virtual ~RenderTextWin();

  // Overridden from RenderText:
  virtual base::i18n::TextDirection GetTextDirection() OVERRIDE;
  virtual Size GetStringSize() OVERRIDE;
  virtual SelectionModel FindCursorPosition(const Point& point) OVERRIDE;
  virtual size_t IndexOfAdjacentGrapheme(
      size_t index,
      LogicalCursorDirection direction) OVERRIDE;

 protected:
  // Overridden from RenderText:
  virtual SelectionModel AdjacentCharSelectionModel(
      const SelectionModel& selection,
      VisualCursorDirection direction) OVERRIDE;
  virtual SelectionModel AdjacentWordSelectionModel(
      const SelectionModel& selection,
      VisualCursorDirection direction) OVERRIDE;
  virtual void SetSelectionModel(const SelectionModel& model) OVERRIDE;
  virtual void GetGlyphBounds(size_t index,
                              ui::Range* xspan,
                              int* height) OVERRIDE;
  virtual std::vector<Rect> GetSubstringBounds(ui::Range range) OVERRIDE;
  virtual bool IsCursorablePosition(size_t position) OVERRIDE;
  virtual void ResetLayout() OVERRIDE;
  virtual void EnsureLayout() OVERRIDE;
  virtual void DrawVisualText(Canvas* canvas) OVERRIDE;

 private:
  void ItemizeLogicalText();
  void LayoutVisualText();

  // Helper function to update the font on a text run after font substitution.
  void ApplySubstituteFont(internal::TextRun* run, const Font& font);

  // Returns whether |run| contains missing glyphs.
  bool HasMissingGlyphs(internal::TextRun* run) const;

  // Returns a vector of linked fonts corresponding to |font|.
  const std::vector<Font>* GetLinkedFonts(const Font& font) const;

  // Return the run index that contains the argument; or the length of the
  // |runs_| vector if argument exceeds the text length or width.
  size_t GetRunContainingCaret(const SelectionModel& caret) const;
  size_t GetRunContainingPoint(const Point& point) const;

  // Given a |run|, returns the SelectionModel that contains the logical first
  // or last caret position inside (not at a boundary of) the run.
  // The returned value represents a cursor/caret position without a selection.
  SelectionModel FirstSelectionModelInsideRun(const internal::TextRun* run);
  SelectionModel LastSelectionModelInsideRun(const internal::TextRun* run);

  // Cached HDC for performing Uniscribe API calls.
  static HDC cached_hdc_;

  // Cached map from font names to vectors of linked fonts.
  static std::map<std::string, std::vector<Font> > cached_linked_fonts_;

  // Cached map of system fonts, from file names to font families.
  static std::map<std::string, std::string> cached_system_fonts_;

  // Cached map from font name to the last successful substitute font used.
  static std::map<std::string, Font> successful_substitute_fonts_;

  SCRIPT_CONTROL script_control_;
  SCRIPT_STATE script_state_;

  std::vector<internal::TextRun*> runs_;
  Size string_size_;

  // A common vertical baseline for all the text runs. This is computed as the
  // largest baseline over all the runs' fonts.
  int common_baseline_;

  scoped_array<int> visual_to_logical_;
  scoped_array<int> logical_to_visual_;

  bool needs_layout_;

  DISALLOW_COPY_AND_ASSIGN(RenderTextWin);
};

}  // namespace gfx

#endif  // UI_GFX_RENDER_TEXT_WIN_H_
