// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MainUnit.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "SurfaceUnit"
#pragma resource "*.dfm"
TMainForm *MainForm;

// ---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner) : TForm(Owner), delEl(NULL)
{
	primitiveManager = new PrimitiveManager(Handle);
	xmlPrContainer.clear();
	if (!primitiveManager)
	{
		Application->Terminate();
	}
	filename = "";
	mouseDown[0] = false;
	mouseDown[1] = false;
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::quitMenuItemClick(TObject *Sender)
{
	Close();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{

	SurfaceRender1->drawEveryPrimitive = this->draw;
	RendererVars &vars = RendererVars::getInstance();
	if (!vars.isSucceseed())
	{
		Application->Terminate();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::draw()
{

	if (primitiveManager)
	{
		primitiveManager->drawPrimitives(Handle);
	}
}

void __fastcall TMainForm::AddLinePrimitiveToolClick(TObject *Sender)
{
	if (!primitiveManager->addPrimitive(new LinePrimitive()))
	{
		ShowMessage("Не удалось создать примитив линия");
		return;
	}
	addPrimitiveToList();
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::AddElliopsePrimitiveToolClick(TObject *Sender)
{
	if (!primitiveManager->addPrimitive(new EllipsePrimitive()))
	{
		ShowMessage("Не удалось создать примитив линия");
		return;
	}
	addPrimitiveToList();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::AddRectPrimitiveToolClick(TObject *Sender)
{
	if (!primitiveManager->addPrimitive(new RectPrimitive()))
	{
		ShowMessage("Не удалось создать примитив линия");
		return;
	}
	addPrimitiveToList();
}

void __fastcall TMainForm::addPrimitiveToList()  //Добавление элемента в список отображения всех примитивов
{
	TXMLPrimitiveElement *el = new TXMLPrimitiveElement(NULL);
	el->Top = el->Height * xmlPrContainer.size();
	el->Width = PrimitiveScrollBox->Width - 10;
	el->Parent = PrimitiveScrollBox;
	xmlPrContainer.push_back(el);
	el->setPrimitiveInfo(primitiveManager->currentPrimitive());
	PrimitiveScrollBox->VertScrollBar->Position = PrimitiveScrollBox->VertScrollBar->Range;
}

void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (primitiveManager)
	{
		delete primitiveManager;
		primitiveManager = NULL;
		SurfaceRender1->ReleaseSurface();
	}
	auto shaders = ShaderPrimitive::getInstance();
	shaders.releaseAllShaders();
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::saveAsToolClick(TObject *Sender)
{
	if (XMLSaveDialog->Execute())
	{
		filename = XMLSaveDialog->FileName;  //Сохранение в файл
		saveToolClick(Sender);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::openXMLToolClick(TObject *Sender)
{
	if (XMLOpenDialog->Execute())
	{
		NewFileToolClick(Sender);
		filename = XMLOpenDialog->FileName;
		auto fn = filename.c_str();
		XMLConverter *xml = new XMLConverter(fn, false);
		if (!xml)
		{
			ShowMessage("Не удалось сохранить");
			return;
		}
		if (!xml->isInit())
		{
			delete xml;
			xml = NULL;
			ShowMessage("Не удалось создать файл");
			return;
		}
		xml->parseDocument(primitiveManager);

		delete xml;
		xml = NULL;
        primitiveManager->resetSkipMove();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::saveToolClick(TObject *Sender)
{
	if (filename == "")
		return;
	auto fn = filename.c_str();
	XMLConverter *xml = new XMLConverter(fn, true);
	if (!xml)
	{
		ShowMessage("Не удалось сохранить");
		return;
	}
	if (!xml->isInit())
	{
		ShowMessage("Не удалось сохранить");
		return;
	}
	auto primitives = primitiveManager->getPrimitives();
	for (auto p : primitives)
	{
		xml->saveElement(p);
	}
	delete xml;
	xml = NULL;
}

// ---------------------------------------------------------------------------
void TMainForm::clearManegerElemetns()
{
	for (auto it = xmlPrContainer.begin(), endit = xmlPrContainer.end(); it < endit; ++it)
	{
		delete(*it);
	}
	xmlPrContainer.clear();
}

void __fastcall TMainForm::NewFileToolClick(TObject *Sender)
{
	clearManegerElemetns();
	filename = "";
	if (primitiveManager)
	{
		delete primitiveManager;
		primitiveManager = NULL;
	}
	primitiveManager = new PrimitiveManager(Handle);
	if (!primitiveManager)
	{
		Application->Terminate();
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::SurfaceRender1RendererMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
	int X, int Y)
{
	if (mouseDown[0] == false && Button == mbLeft)
	{
		mouseDown[0] = true;
		int dx = X;
		int dy = Y;
		SurfaceRender1->getDivXY(dx, dy);
		//GetAsyncKeyState - проверяем нажата ли клавиша Ctrl (проверяется как левая, так и правая)
		primitiveManager->leftMouseDown(dx, dy, GetAsyncKeyState(VK_LCONTROL) | GetAsyncKeyState(VK_RCONTROL));
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::SurfaceRender1RendererMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
	int X, int Y)
{
	if (Button == mbLeft)
	{
		mouseDown[0] = false;
	}
	primitiveManager->resetCursor(X, Y);
	float x, y;
	primitiveManager->getMaxXY(x, y);
	SurfaceRender1->setScrollRange(x, y);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::SurfaceRender1RendererMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	int dx = X;
	int dy = Y;
	SurfaceRender1->getDivXY(dx, dy);
	primitiveManager->moveCursor(dx, dy);
}

// ---------------------------------------------------------------------------
void __fastcall TMainForm::deleteElement(TMessage &msg)
{
	Primitive *p = (Primitive*)msg.WParam;
	for (auto x = xmlPrContainer.begin(), en = xmlPrContainer.end(); x < en; ++x)
	{
		if ((*x)->getPrimitive() == p)
		{
			(*x)->Free();
			xmlPrContainer.erase(x);
			break;
		}
	}
	if (xmlPrContainer.empty())
	{
		return;
	}
	int d = 0;
	for (auto x : xmlPrContainer)
	{
		x->Top = d;
		d += x->Height;
	}
}

void __fastcall TMainForm::SurfaceRender1RendererDblClick(TObject *Sender)
{
	auto p = primitiveManager->getPrimitiveUnderCursor();
	if (p)
	{
		SettingStyleDialog->setPrimitive(p);
		p->resetUnderCursor();
		SettingStyleDialog->ShowModal();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainForm::changeCursor(TMessage &msg)
{
	switch ((EditMode)msg.WParam)
	{
	case EditMode::Move:
		SurfaceRender1->Renderer->Cursor = crHandPoint;
		break;
	case EditMode::Rotate:
		SurfaceRender1->Renderer->Cursor = crCross;
		break;

	case EditMode::ResizeY0:
	case EditMode::ResizeY1:
		SurfaceRender1->Renderer->Cursor = crSizeNS;
		break;
	case EditMode::ResizeX0:
	case EditMode::ResizeX1:
		SurfaceRender1->Renderer->Cursor = crSizeWE;
		break;
	case EditMode::LChgXY0:
	case EditMode::LChgXY1:
		SurfaceRender1->Renderer->Cursor = crSizeAll;
		break;
	default:
		SurfaceRender1->Renderer->Cursor = crDefault;
	}
}
