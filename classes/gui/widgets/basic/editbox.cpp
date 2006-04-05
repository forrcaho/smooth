 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/editbox.h>
#include <smooth/gui/widgets/special/cursor.h>
#include <smooth/gui/widgets/multi/list/combobox.h>
#include <smooth/misc/binary.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::EditBox::classID = S::Object::RequestClassID();

S::GUI::EditBox::EditBox(const String &iText, const Point &iPos, const Size &iSize, Int maxSize) : Widget(iPos, iSize)
{
	type		= classID;

	dropDownList	= NIL;
	comboBox	= NIL;

	font.SetColor(Setup::ClientTextColor);

	if (GetWidth() == 0) SetWidth(80);
	if (GetHeight() == 0) SetHeight(19);

	cursor = new Cursor(Point(3, 2), GetSize() - Size(6, 4));
	cursor->SetMaxSize(maxSize);
	cursor->SetBackgroundColor(Setup::ClientColor);
	cursor->SetFont(font);
	cursor->SetText(iText);
	cursor->onInput.Connect(&onInput);
	cursor->onEnter.Connect(&onEnter);

	RegisterObject(cursor);

	onInput.SetParentObject(this);
	onEnter.SetParentObject(this);

	onChangeSize.Connect(&EditBox::OnChangeSize, this);
}

S::GUI::EditBox::~EditBox()
{
	DeleteObject(cursor);

	if (comboBox != NIL) DeleteObject(comboBox);
}

S::Int S::GUI::EditBox::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				Surface	*surface = container->GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetSize());

				if (IsActive())	surface->Box(frame, Setup::ClientColor, FILLED);
				else		surface->Box(frame, Setup::BackgroundColor, FILLED);

				surface->Frame(frame, FRAME_DOWN);
			}

			break;
	}

	return Widget::Paint(message);
}

S::Int S::GUI::EditBox::MarkAll()
{
	return cursor->MarkAll();
}

S::Int S::GUI::EditBox::Activate()
{
	cursor->SetBackgroundColor(Setup::ClientColor);

	return Widget::Activate();
}

S::Int S::GUI::EditBox::Deactivate()
{
	cursor->SetBackgroundColor(Setup::BackgroundColor);

	return Widget::Deactivate();
}

S::Int S::GUI::EditBox::SetText(const String &newText)
{
	return cursor->SetText(newText);
}

const S::String &S::GUI::EditBox::GetText()
{
	return cursor->GetText();
}

S::Int S::GUI::EditBox::SetDropDownList(List *nDropDownList)
{
	if (comboBox != NIL) DeleteObject(comboBox);

	dropDownList = nDropDownList;

	if (dropDownList != NIL)
	{
		comboBox = new ComboBox(Point(), Size());
		comboBox->SetFlags(CB_HOTSPOTONLY);
		comboBox->onSelectEntry.Connect(&EditBox::OnSelectListEntry, this);

		for (Int i = 0; i < dropDownList->GetNOfEntries(); i++)
		{
			comboBox->AddEntry(dropDownList->GetNthEntry(i)->GetText());
		}

		RegisterObject(comboBox);

		OnChangeSize(GetSize());
	}

	return Success();
}

S::Void S::GUI::EditBox::OnSelectListEntry(ListEntry *entry)
{
	cursor->SetText(entry->GetText());
	cursor->MarkAll();

	onInput.Emit(cursor->GetText());
}

S::Int S::GUI::EditBox::GetCursorPos()
{
	return cursor->GetCursorPos();
}

S::Void S::GUI::EditBox::OnChangeSize(const Size &nSize)
{
	cursor->SetSize(nSize - Size(6 + (comboBox != NIL ? 17 : 0), 4));

	if (comboBox != NIL) comboBox->SetSize(nSize);
}