 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/list/listentry.h>
#include <smooth/gui/widgets/multi/list/listbox.h>
#include <smooth/gui/widgets/hotspot/hotspot.h>
#include <smooth/gui/window/window.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>
#include <smooth/misc/binary.h>

S::Signal2<S::Void, S::Int, S::Int>	 S::GUI::ListEntry::internalOnSelectEntry;

const S::Int	 S::GUI::ListEntry::classID = S::Object::RequestClassID();

S::GUI::ListEntry::ListEntry(const String &iText) : Widget(Point(), Size(100, 15))
{
	type		= classID;

	selected	= False;
	marked		= False;
	text		= iText;

	font.SetColor(Setup::ClientTextColor);

	hotspot		= new Hotspot(Point(), GetSize());
	markHotspot	= new Hotspot(Point(2, 3), Size(9, 9));

	hotspot->onLeftButtonDown.Connect(&ListEntry::OnSelectEntry, this);
	markHotspot->onLeftButtonDown.Connect(&ListEntry::OnToggleMark, this);

	RegisterObject(hotspot);
	RegisterObject(markHotspot);

	internalOnSelectEntry.Connect(&ListEntry::OnSelectOtherEntry, this);

	onChangeSize.Connect(&Hotspot::SetSize, hotspot);
}

S::GUI::ListEntry::~ListEntry()
{
	internalOnSelectEntry.Disconnect(&ListEntry::OnSelectOtherEntry, this);

	DeleteObject(hotspot);
	DeleteObject(markHotspot);
}

S::Int S::GUI::ListEntry::Hide()
{
	visible = False;

	return Success();
}

S::Int S::GUI::ListEntry::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
		case SP_MOUSEIN:
		case SP_MOUSEOUT:
			{
				if (container->GetObjectType() != ListBox::classID) return Success();

				Surface	*surface = container->GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetSize());
				Font	 nFont	 = font;
				Bool	 gotTabs = False;

				if (mouseOver) nFont.SetColor(Setup::GradientTextColor);

				for (Int r = 0; r < text.Length(); r++) if (text[r] == '\t') { gotTabs = True; break; }

				surface->StartPaint(frame);

				if (mouseOver)	surface->Gradient(frame, Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);
				else		surface->Box(frame, Setup::ClientColor, FILLED);

				if (selected)	surface->Box(frame, Setup::ClientTextColor, OUTLINEDOTS);

				if (container->GetFlags() & LF_MULTICHECKBOX)
				{
					Rect	 cbRect = Rect(GetRealPosition() + Point(2, 3), Size(9, 9));

					if (cbRect.top <= cbRect.bottom - 1)
					{
						surface->Box(cbRect, Setup::ClientColor, FILLED);
						surface->Box(cbRect, Setup::GrayTextColor, OUTLINED);

						if (marked && cbRect.top <= cbRect.bottom - 3)
						{
							Point	 p1 = Point(cbRect.left + 2 + (Setup::rightToLeft ? 1 : 0), cbRect.top + 2);
							Point	 p2 = Point(cbRect.right - 2 + (Setup::rightToLeft ? 1 : 0), cbRect.bottom - 2);

							surface->Line(p1, p2, Setup::ClientTextColor);
							surface->Line(p1 + Point(1, 0), p2 + Point(0, -1), Setup::GrayTextColor);
							surface->Line(p1 + Point(0, 1), p2 + Point(-1, 0), Setup::GrayTextColor);
							surface->Line(p1 + Point(4, 0), p2 + Point(-6, 0), Setup::ClientTextColor);
							surface->Line(p1 + Point(4, 1), p2 + Point(-5, 0), Setup::GrayTextColor);
							surface->Line(p1 + Point(3, 0), p2 + Point(-6, -1), Setup::GrayTextColor);
						}
					}
				}

				if (((ListBox *) container)->GetNOfTabs() > 0 && gotTabs)
				{
					for (Int i = 0; i < ((ListBox *) container)->GetNOfTabs(); i++)
					{
						String	 tabText;
						Rect	 rect = Rect(GetRealPosition() + Point(1, 1), GetSize() - Size(3, 2));

						rect.left += ((ListBox *) container)->GetNthTabOffset(i);
						rect.left += (i == 0 ? (container->GetFlags() & LF_MULTICHECKBOX ? 12 : 0) : 0);

						if (((ListBox *) container)->GetNOfTabs() >= i + 2) rect.right = rect.left + (((ListBox *) container)->GetNthTabOffset(i + 1) - ((ListBox *) container)->GetNthTabOffset(i)) - (i == 0 ? (container->GetFlags() & LF_MULTICHECKBOX ? 12 : 0) : 0) - 3;

						Int	 tabCount = 0;

						for (Int p = 0; p < text.Length(); p++)
						{
							if (tabCount == i)
							{
								for (Int q = p; q < text.Length(); q++)
								{
									if (text[q] == '\t') break;

									tabText[q - p] = text[q];
								}

								break;
							}

							if (text[p] == '\t') tabCount++;
						}

						if (((ListBox *) container)->GetNthTabOrientation(i) == OR_RIGHT)
						{
							rect.left = Math::Max(rect.left, rect.right - nFont.GetTextSizeX(tabText));
						}

						surface->SetText(tabText, rect, nFont);
					}
				}
				else
				{
					surface->SetText(text, frame + Point(1 + (container->GetFlags() & LF_MULTICHECKBOX ? 12 : 0), 1) - Size(2 + (container->GetFlags() & LF_MULTICHECKBOX ? 12 : 0), 2), nFont);
				}

				surface->EndPaint();
			}

			break;
	}

	return Success();
}

S::Int S::GUI::ListEntry::SetMark(Bool nMarked)
{
	marked = nMarked;

	Paint(SP_PAINT);

	return Success();
}

S::Bool S::GUI::ListEntry::IsMarked()
{
	return marked;
}

S::Int S::GUI::ListEntry::Select()
{
	selected = True;

	internalOnSelectEntry.Emit(container->GetHandle(), GetHandle());

	Paint(SP_PAINT);	

	((List *) container)->onSelectEntry.Emit(this);

	return Success();
}

S::Int S::GUI::ListEntry::Deselect()
{
	selected = False;

	Paint(SP_PAINT);

	return Success();
}

S::Bool S::GUI::ListEntry::IsSelected()
{
	return selected;
}

S::Void S::GUI::ListEntry::OnToggleMark()
{
	SetMark(!marked);

	((List *) container)->onMarkEntry.Emit(this);
}

S::Void S::GUI::ListEntry::OnSelectEntry()
{
	if (!selected || Binary::IsFlagSet(container->GetFlags(), LF_ALLOWRESELECT)) Select();
}

S::Void S::GUI::ListEntry::OnSelectOtherEntry(Int containerHandle, Int handle)
{
	if (container->GetHandle() == containerHandle && GetHandle() != handle && selected) Deselect();
}