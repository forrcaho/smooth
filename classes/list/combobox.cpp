 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/combobox.h>
#include <smooth/listbox.h>
#include <smooth/list.h>
#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/loop.h>
#include <smooth/objectmanager.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/i18n.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/toolwindow.h>
#include <smooth/layer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_COMBOBOX = S::Object::RequestObjectID();

S::ComboBox::ComboBox(Point pos, Size size, ProcParam, Void *procParam)
{
	type				= OBJ_COMBOBOX;
	objectProperties->proc		= (ProcType) newProc;
	objectProperties->procParam	= procParam;
	entryCount			= -1;

	closeListBox	= False;
	listBoxOpen	= False;
	listBox		= NIL;
	toolWindow	= NIL;
	layer		= NIL;

	possibleContainers.AddEntry(OBJ_LAYER);

	SetFont(objectProperties->font, I18N_SMALLFONTSIZE, SMOOTH::Setup::ClientTextColor, objectProperties->fontWeight);

	objectProperties->pos.x = Math::Round(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(19 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * SMOOTH::Setup::FontSize);
}

S::ComboBox::~ComboBox()
{
	if (listBoxOpen)
	{
		delete listBox;
	}

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::List::Entry *S::ComboBox::AddEntry(String name, ProcParam, Void *procParam)
{
	entryCount++;

	Entry *newEntry = AddListEntry(entryCount, name, newProc, procParam);

	if (entryCount == 0) entries.GetFirstEntry()->clk = True;

	Paint(SP_PAINT);

	return newEntry;
}

S::Int S::ComboBox::ModifyEntry(Int code, String name, ProcParam, Void *procParam)
{
	if (ModifyListEntry(code, name, newProc, procParam) == Success)
	{
		Paint(SP_PAINT);

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Int S::ComboBox::RemoveEntry(Int number)
{
	RemoveListEntry(number);

	Paint(SP_PAINT);

	return Success;
}

S::Void S::ComboBox::Cleanup()
{
	CleanupList();

	Paint(SP_PAINT);
}

S::Int S::ComboBox::SelectEntry(Int code)
{
	SelectListEntry(code);

	Paint(SP_PAINT);

	return Success;
}

S::Int S::ComboBox::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Layer	*lay = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) lay->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	HDC		 dc = GetContext(wnd);
	Point		 realPos = GetRealPosition();
	List::Entry	*operat;
	Rect		 frame;
	Point		 lineStart;
	Point		 lineEnd;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (active)	Box(dc, frame, SMOOTH::Setup::ClientColor, FILLED);
	else		Box(dc, frame, SMOOTH::Setup::BackgroundColor, FILLED);

	Frame(dc, frame, FRAME_DOWN);

	frame.top++;
	frame.bottom--;
	frame.right--;
	frame.left = frame.right - METRIC_COMBOBOXOFFSETX;

	Box(dc, frame, SMOOTH::Setup::BackgroundColor, FILLED);
	Frame(dc, frame, FRAME_UP);

	frame.top--;
	frame.bottom++;
	frame.right++;
	frame.left = realPos.x;

	lineStart.x = frame.right - METRIC_COMBOBOXARROWOFFSETX;
	lineStart.y = frame.top + METRIC_COMBOBOXARROWOFFSETY;
	lineEnd.x = lineStart.x + 7;
	lineEnd.y = lineStart.y;

	for (Int i = 0; i < 4; i++)
	{
		if (active)	Line(dc, lineStart, lineEnd, SMOOTH::Setup::TextColor, PS_SOLID, 1);
		else		Line(dc, lineStart, lineEnd, SMOOTH::Setup::GrayTextColor, PS_SOLID, 1);

		lineStart.x++;
		lineStart.y++;
		lineEnd.x--;
		lineEnd.y++;
	}

	for (Int j = 0; j < nOfEntries; j++)
	{
		operat = entries.GetNthEntry(j);

		if (operat->clk)
		{
			frame.left	+= METRIC_COMBOBOXTEXTOFFSETXY;
			frame.top	+= METRIC_COMBOBOXTEXTOFFSETXY;
			frame.right	-= (METRIC_COMBOBOXOFFSETX + 2);

			::SetText(dc, operat->text, frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

			frame.right	+= (METRIC_COMBOBOXOFFSETX + 2);
			frame.left	-= METRIC_COMBOBOXTEXTOFFSETXY;
			frame.top	-= METRIC_COMBOBOXTEXTOFFSETXY;
		}
	}

	FreeContext(wnd, dc);

	return Success;
}

S::Int S::ComboBox::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Layer	*lay = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) lay->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Point		 realPos = GetRealPosition();
	Int		 retVal = Success;
	List::Entry	*operat;
	Rect		 frame;
	Rect		 lbframe;
	HDC		 dc;
	Point		 lbp;
	Size		 lbs;
	Bool		 executeProcs = False;
	Float		 oldMeasurement;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	switch (message)
	{
#ifdef __WIN32__
		case WM_ACTIVATE:
			if (message == WM_ACTIVATE) if (toolWindow != NIL) if (((HWND) lParam) == toolWindow->hwnd) break;
		case WM_ACTIVATEAPP:
		case WM_KILLFOCUS:
			if (message == WM_KILLFOCUS) if (toolWindow != NIL) if (((HWND) wParam) == toolWindow->hwnd) break;

			if (listBoxOpen)
			{
				listBoxOpen = False;

				wnd->UnregisterObject(toolWindow);
				toolWindow->UnregisterObject(layer);
				layer->UnregisterObject(listBox);

				toolWindow->FreeOwner();

				delete listBox;
				delete layer;
				delete toolWindow;

				listBox		= NIL;
				layer		= NIL;
				toolWindow	= NIL;
			}

			break;
#endif
		case SM_LOOSEFOCUS:
			lbframe.top	= frame.bottom + 1;
			lbframe.bottom	= lbframe.top + min(METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);
			lbframe.right	= frame.right;
			lbframe.left	= frame.left;

			if (wParam != handle)
			{
				if ((IsMouseOn(wnd->hwnd, frame, WINDOW) && listBoxOpen) || (!IsMouseOn(wnd->hwnd, frame, WINDOW) && !IsMouseOn(wnd->hwnd, lbframe, WINDOW) && listBoxOpen))
				{
					listBoxOpen = False;

					wnd->UnregisterObject(toolWindow);
					toolWindow->UnregisterObject(layer);
					layer->UnregisterObject(listBox);

					toolWindow->FreeOwner();

					delete listBox;
					delete layer;
					delete toolWindow;

					listBox		= NIL;
					layer		= NIL;
					toolWindow	= NIL;

					retVal = Break;
				}
			}

			break;
		case SM_LBUTTONDOWN:
			dc = GetContext(wnd);

			lbframe.top	= frame.bottom + 1;
			lbframe.bottom	= lbframe.top + min(METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);
			lbframe.right	= frame.right;
			lbframe.left	= frame.left;

			if (IsMouseOn(wnd->hwnd, frame, WINDOW) && !listBoxOpen)
			{
				wnd->Process(SM_LOOSEFOCUS, handle, 0);

				listBoxOpen = True;

				lbp.x = frame.left - lay->GetObjectProperties()->pos.x;
				lbp.y = frame.bottom + 1 - lay->GetObjectProperties()->pos.y;
				lbs.cx = objectProperties->size.cx;
				lbs.cy = min(METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);

				oldMeasurement = SMOOTH::Setup::FontSize;

				SetMeasurement(SMT_PIXELS);

				layer		= new Layer();
				toolWindow	= new ToolWindow();
				listBox		= new ListBox(Point(0, 0), lbs, Proc(&ComboBox::ListBoxProc), this);

				lbp.x = frame.left + wnd->GetObjectProperties()->pos.x;
				lbp.y = frame.bottom + 1 + wnd->GetObjectProperties()->pos.y;

				if (objectProperties->checked)
				{
					objectProperties->clicked = True;

					frame.top	+= 3;
					frame.bottom	-= 3;
					frame.right	-= 3;
					frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

					Frame(dc, frame, FRAME_DOWN);
				}

				toolWindow->SetMetrics(lbp, lbs);

				SMOOTH::Setup::FontSize = oldMeasurement;

				listBox->AllowReselect(True);

				for (Int i = 0; i < nOfEntries; i++)
				{
					operat = entries.GetNthEntry(i);

					listBox->AddEntry(operat->text, NULLPROC);
				}

				listBox->SelectEntry(GetSelectedEntry());

				wnd->RegisterObject(toolWindow);
				toolWindow->RegisterObject(layer);
				layer->RegisterObject(listBox);

				toolWindow->SetOwner(this);

				toolWindow->Show();

				listBox->Paint(SP_PAINT);

				retVal = Break;
			}
			else if ((IsMouseOn(wnd->hwnd, frame, WINDOW) && listBoxOpen) || (!IsMouseOn(wnd->hwnd, frame, WINDOW) && !IsMouseOn(wnd->hwnd, lbframe, WINDOW) && listBoxOpen))
			{
				listBoxOpen = False;

				if (objectProperties->checked)
				{
					objectProperties->clicked = True;

					frame.top	+= 3;
					frame.bottom	-= 3;
					frame.right	-= 3;
					frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

					Frame(dc, frame, FRAME_DOWN);

					frame.top	-= 3;
					frame.bottom	+= 3;
					frame.right	+= 3;
					frame.left	= realPos.x;
				}

				frame.top	= frame.bottom + 1;
				frame.bottom	= frame.top + min(METRIC_LISTBOXENTRYHEIGHT * nOfEntries + 4, METRIC_LISTBOXENTRYHEIGHT * 5 + 4);
				frame.right++;

				wnd->UnregisterObject(toolWindow);
				toolWindow->UnregisterObject(layer);
				layer->UnregisterObject(listBox);

				toolWindow->FreeOwner();

				delete listBox;
				delete layer;
				delete toolWindow;

				listBox		= NIL;
				layer		= NIL;
				toolWindow	= NIL;

				frame.left	= realPos.x;
				frame.top	= realPos.y;
				frame.right	= realPos.x + objectProperties->size.cx - 1;
				frame.bottom	= realPos.y + objectProperties->size.cy - 1;

				if (!IsMouseOn(wnd->hwnd, frame, WINDOW) && !IsMouseOn(wnd->hwnd, lbframe, WINDOW)) wnd->Process(SM_LBUTTONDOWN, 0, 0);

				retVal = Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_LBUTTONUP:
			dc = GetContext(wnd);

			if (closeListBox)
			{
				for (Int i = 0; i < nOfEntries; i++)
				{
					operat = entries.GetNthEntry(i);

					if (operat->clk)
					{
						frame.left	+= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.top	+= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.right	-= (METRIC_COMBOBOXOFFSETX + 2);

						::SetText(dc, operat->text, frame, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::ClientColor, objectProperties->fontWeight);

						frame.right	+= (METRIC_COMBOBOXOFFSETX + 2);
						frame.left	-= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.top	-= METRIC_COMBOBOXTEXTOFFSETXY;
					}
				}

				if (GetSelectedEntry() != listBox->GetSelectedEntry()) executeProcs = True;

				SelectEntry(listBox->GetSelectedEntry());

				for (Int j = 0; j < nOfEntries; j++)
				{
					operat = entries.GetNthEntry(j);

					if (operat->clk)
					{
						frame.left	+= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.top	+= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.right	-= (METRIC_COMBOBOXOFFSETX + 2);

						::SetText(dc, operat->text, frame, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

						frame.right	+= (METRIC_COMBOBOXOFFSETX + 2);
						frame.left	-= METRIC_COMBOBOXTEXTOFFSETXY;
						frame.top	-= METRIC_COMBOBOXTEXTOFFSETXY;
					}
				}

				if (listBoxOpen)
				{
					listBoxOpen = False;

					wnd->UnregisterObject(toolWindow);
					toolWindow->UnregisterObject(layer);
					layer->UnregisterObject(listBox);

					toolWindow->FreeOwner();

					delete listBox;
					delete layer;
					delete toolWindow;

					listBox		= NIL;
					layer		= NIL;
					toolWindow	= NIL;
				}

				closeListBox = False;

				retVal = Break;
			}

			if (executeProcs)
			{
				for (Int i = 0; i < nOfEntries; i++)
				{
					operat = entries.GetNthEntry(i);

					if (operat->clk)
					{
						ProcCall(objectProperties->proc, objectProperties->procParam);
						ProcCall(operat->proc, operat->procParam);

						break;
					}
				}
			}

			if (objectProperties->clicked)
			{
				objectProperties->clicked = False;

				if (objectProperties->checked)
				{
					frame.top	+= 3;
					frame.bottom	-= 3;
					frame.right	-= 3;
					frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

					Frame(dc, frame, FRAME_UP);
				}
				else
				{
					frame.top	+= 3;
					frame.bottom	-= 2;
					frame.right	-= 2;
					frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 3;

					Box(dc, frame, SMOOTH::Setup::BackgroundColor, OUTLINED);
				}
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSELEAVE:
			dc = GetContext(wnd);

			frame.top	+= 3;
			frame.bottom	-= 3;
			frame.right	-= 3;
			frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

			if (!IsMouseOn(wnd->hwnd, frame, WINDOW) && objectProperties->checked)
			{
				frame.right++;
				frame.bottom++;

				Box(dc, frame, SMOOTH::Setup::BackgroundColor, OUTLINED);

				objectProperties->checked = False;
				objectProperties->clicked = False;
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEMOVE:
			dc = GetContext(wnd);

			frame.top	+= 3;
			frame.bottom	-= 3;
			frame.right	-= 3;
			frame.left	= frame.right - METRIC_COMBOBOXOFFSETX + 4;

			if (IsMouseOn(wnd->hwnd, frame, WINDOW) && !objectProperties->checked)
			{
				Frame(dc, frame, FRAME_UP);

				objectProperties->checked = True;
			}
			else if (!IsMouseOn(wnd->hwnd, frame, WINDOW) && objectProperties->checked)
			{
				frame.right++;
				frame.bottom++;

				Box(dc, frame, SMOOTH::Setup::BackgroundColor, OUTLINED);

				objectProperties->checked = False;
				objectProperties->clicked = False;
			}

			FreeContext(wnd, dc);

			break;
		case SM_CHECKCOMBOBOXES:
			dc = GetContext(wnd);

			if (listBox != NIL)
			{
				closeListBox = True;
				
				toolWindow->SetOwner(this);

				retVal = Break;
			}

			FreeContext(wnd, dc);

			break;
	}

	return retVal;
}

S::Void S::ComboBox::ListBoxProc()
{
	Process(SM_CHECKCOMBOBOXES, 0, 0);
}
