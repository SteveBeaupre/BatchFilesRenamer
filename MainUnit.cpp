//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
CScriptEngine ScriptEngine;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::InitializeUI()
{
	ListViewFilesList->Align = alClient;
	RichEditScriptText->Align = alClient;
	RichEditScriptOutput->Align = alClient;
	FilesListPanel->Align = alClient;
	ScriptPanel->Align = alClient;
	ScriptPanel->SendToBack();
	FilesListPanel->Color = clBtnFace;
	ScriptPanel->Color = clBtnFace;
	ScriptTextPanel->Color = clBtnFace;
	ScriptOutputPanel->Color = clBtnFace;
	TabControl->OnChange(this);
	HidePathsMenu->OnClick(this);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender)
{
	static bool Init = false;
	if(!Init){
		HidePathsMenu->OnClick(this);
		Init = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	DragAcceptFiles(Handle, TRUE);

	#ifdef _DEBUG
	UnicodeString Dir = "C:\\Temp\\Batch Files Renamer\\";
	SetCurrentDirectory(Dir.c_str());
	AnsiString AppDir = Dir;
	#else
	AnsiString AppDir = ExtractFilePath(Application->ExeName);
	#endif

	LuaPlugin = AppDir + "LuaPlugin.dll";
	PythonPlugin = AppDir + "PythonPlugin.dll";

	ScriptEngine.Load(PythonPlugin.c_str());

	InitializeUI();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	ScriptEngine.Free();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CloseMenuClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::TabControlChange(TObject *Sender)
{
	FilesListPanel->Visible = TabControl->TabIndex == 0;
	ScriptPanel->Visible = TabControl->TabIndex == 1;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::HidePathsMenuClick(TObject *Sender)
{
	HidePathsMenu->Checked = !HidePathsMenu->Checked;

	// This show or hide columns 0 and 1
	bool Checked = HidePathsMenu->Checked;
	int w = ListViewFilesList->Column[Checked ? 0 : 1]->Width;
	ListViewFilesList->Column[Checked ? 0 : 1]->Width = 0;
	ListViewFilesList->Column[Checked ? 1 : 0]->Width = w;
	for(int i = 0; i < 2; i++){
		ListViewFilesList->Column[i]->AutoSize = false;
		ListViewFilesList->Column[i]->AutoSize = true;
	}

	// This add the path from the original file name to the modified file name in the list view
	for(int i = 0; i < ListViewFilesList->Items->Count; i++){

		TListItem *item = ListViewFilesList->Items->Item[i];
		AnsiString szPath = "";

		if(item->Caption.Length() == 0)
			continue;

		if(!Checked)
			szPath = ExtractFilePath(item->Caption);

		AnsiString szName = ExtractFileName(item->SubItems->Strings[1]);
		if(szName.Length() > 0)
			item->SubItems->Strings[1] = szPath + szName;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::IsEntryDuplicated(UnicodeString FileName)
{
	for(int i = 0; i < ListViewFilesList->Items->Count; i++){
		TListItem *item = ListViewFilesList->Items->Item[i];
		if(item->Caption == FileName)
			return true;
	}

	return false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::AddListEntry(UnicodeString FileName)
{
	if(!IsEntryDuplicated(FileName)){
		TListItem *item = ListViewFilesList->Items->Add();
		item->Caption = FileName;
		item->SubItems->Add(ExtractFileName(FileName));
		item->SubItems->Add("");
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ListViewFilesListKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_DELETE)
		ListViewFilesList->DeleteSelected();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::WndProc(Messages::TMessage &Message)
{
	switch(Message.Msg)
	{
	case WM_DROPFILES:
		if(TabControl->TabIndex != 0)
			break;

		HDROP hDrop = (HDROP)Message.WParam;
		UINT NumFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, NULL);

		for(UINT i = 0; i < NumFiles; i++){

			char FileName[MAX_PATH];
			ZeroMemory(FileName, MAX_PATH);
			DragQueryFileA(hDrop, i, FileName, MAX_PATH);

			AnsiString fName = FileName;

			TSearchRec SearchRec;
			int Result = FindFirst(fName, faAnyFile, SearchRec);

			if(Result == 0){
				if((SearchRec.Attr & faDirectory) == 0){
					AddListEntry(UnicodeString(fName));
				} else {
					RecurseDirectory(fName.c_str(), true);
				}
			}
		}

		DragFinish(hDrop);
		break;
	}

	TForm::WndProc(Message);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RecurseDirectory(char *Dir, bool Recurse)
{
	AnsiString path = AnsiString(Dir) + "\\*.*";

	TSearchRec SearchRec;
	int Result = FindFirst(path, faAnyFile, SearchRec);

	while(Result == 0){

		AnsiString s = AnsiString(Dir) + "\\" + SearchRec.Name;

		if(!(SearchRec.Name == "." || SearchRec.Name == "..")){

			if((SearchRec.Attr & faDirectory) == 0){
				AddListEntry(UnicodeString(s));
			} else {
				if(Recurse)
					RecurseDirectory(s.c_str(), Recurse);
			}
		}

		Result = FindNext(SearchRec);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::AddFilesMenuClick(TObject *Sender)
{
	OpenDialog->Options.Clear();
	OpenDialog->Options << ofHideReadOnly << ofAllowMultiSelect << ofEnableSizing;
	OpenDialog->Filter = "All files (*.*)|*.*";

	char *InitDir = ScriptEngine.GetInitialDirectory();
	if(DirectoryExists(InitDir))
		OpenDialog->InitialDir = InitDir;

	if(OpenDialog->Execute()){
		int FilesCount = OpenDialog->Files->Count;
		for(int i = 0; i < FilesCount; i++)
			AddListEntry(UnicodeString(OpenDialog->Files->Strings[i]));
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::AddFoldersMenuClick(TObject *Sender)
{
	char *InitDir = ScriptEngine.GetInitialDirectory();
	if(DirectoryExists(InitDir))
		AddDirectoryForm->DirectoryListBox->Directory = InitDir;

	if(AddDirectoryForm->ShowModal() == mrOk){
		AnsiString Dir = AddDirectoryForm->DirectoryListBox->Directory;
		RecurseDirectory(Dir.c_str(), AddDirectoryForm->CheckBoxRecurse->Checked);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadListMenuClick(TObject *Sender)
{
	OpenDialog->Options.Clear();
	OpenDialog->Options << ofHideReadOnly << ofEnableSizing;
	OpenDialog->Filter = "Text files (*.txt*)|*.TXT*";

	if(OpenDialog->Execute()){

		AnsiString fname = OpenDialog->FileName;

		FILE *f = fopen(fname.c_str(), "rt");
		if(f){

			ListViewFilesList->Clear();

			while(1){
				char Buf[MAX_PATH];
				ZeroMemory(Buf, MAX_PATH);

				if(fgets(Buf, MAX_PATH, f) == NULL)
					break;

				AddListEntry(UnicodeString(Buf));
			}

			fclose(f);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveListMenuClick(TObject *Sender)
{
	SaveDialog->Options.Clear();
	SaveDialog->Options << ofOverwritePrompt << ofHideReadOnly << ofEnableSizing;
	SaveDialog->Filter = "Text files (*.txt*)|*.TXT*";
	SaveDialog->DefaultExt = "txt";
	if(SaveDialog->Execute()){

		AnsiString fname = SaveDialog->FileName;

		FILE *f = fopen(fname.c_str(), "wt+");
		if(f){

			for(int i = 0; i < ListViewFilesList->Items->Count; i++){
				TListItem *item = ListViewFilesList->Items->Item[i];
				AnsiString s = item->Caption;
				fputs(s.c_str(), f);
			}

			fclose(f);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LuaMenuClick(TObject *Sender)
{
	TMenuItem* pmi = dynamic_cast<TMenuItem*>(Sender);
	switch(pmi->Tag)
	{
	case 0:
		if(!LuaMenu->Checked){
			LuaMenu->Checked = true;
			PythonMenu->Checked = false;
			ScriptEngine.Free();
			ScriptEngine.Load(LuaPlugin.c_str());
			RichEditScriptText->Lines->Clear();
		}
		break;
	case 1:
		if(!PythonMenu->Checked){
			PythonMenu->Checked = true;
			LuaMenu->Checked = false;
			ScriptEngine.Free();
			ScriptEngine.Load(PythonPlugin.c_str());
			RichEditScriptText->Lines->Clear();
		}
		break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadScriptMenuClick(TObject *Sender)
{
	OpenDialog->Options.Clear();
	OpenDialog->Options << ofHideReadOnly << ofEnableSizing;
	switch(LuaMenu->Checked)
	{
	case true:  OpenDialog->Filter = "Lua files (*.lua*)|*.LUA*";  break;
	case false: OpenDialog->Filter = "Python files (*.py*)|*.PY*"; break;
	}
	if(OpenDialog->Execute()){
		RichEditScriptText->Lines->LoadFromFile(OpenDialog->FileName);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveScriptMenuClick(TObject *Sender)
{
	SaveDialog->Options.Clear();
	SaveDialog->Options << ofOverwritePrompt << ofHideReadOnly << ofEnableSizing;
	switch(LuaMenu->Checked)
	{
	case true:
		SaveDialog->Filter = "Lua files (*.lua*)|*.LUA*";
		SaveDialog->DefaultExt = "lua";
		break;
	case false:
		SaveDialog->Filter = "Python files (*.py*)|*.PY*";
		SaveDialog->DefaultExt = "py";
		break;
	}
	if(SaveDialog->Execute()){
		RichEditScriptText->Lines->SaveToFile(SaveDialog->FileName);
	}
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::RunScript(bool Simulate)
{
	// Clear the output window
	RichEditScriptOutput->Lines->Clear();

	// Load the script
	AnsiString s = RichEditScriptText->Text.c_str();
	ScriptEngine.LoadScript(s.c_str());

	// Output some pre-defined globals variables from the script
	RichEditScriptOutput->Lines->Add("Language Name: " + UnicodeString(ScriptEngine.GetLanguageName()));
	RichEditScriptOutput->Lines->Add("Script Name: " + UnicodeString(ScriptEngine.GetScriptName()));
	RichEditScriptOutput->Lines->Add("Initial Directory Name: " + UnicodeString(ScriptEngine.GetInitialDirectory()));
	RichEditScriptOutput->Lines->Add("");

	// Store the nuber of items
	int NumItems = ListViewFilesList->Items->Count;

	// For all items...
	for(int i = 0; i < NumItems; i++){

		// Text Buffers
		char ErrMsgBuffer[1024];
		char ModifiedFileName[MAX_PATH];
		ZeroMemory(&ErrMsgBuffer[0], 1024);
		ZeroMemory(&ModifiedFileName[0], MAX_PATH);

		// Store the original file name
		TListItem *item = ListViewFilesList->Items->Item[i];
		AnsiString szOriginalFileName = ExtractFileName(item->Caption);
		AnsiString szOriginalFilePath = ExtractFilePath(item->Caption);

		// Generate the modified name by calling the script's Convert() function
		bool res = ScriptEngine.Convert(szOriginalFileName.c_str(), ModifiedFileName, ErrMsgBuffer);

		// Return false on error
		if(!res || ErrMsgBuffer[0] != NULL){
			RichEditScriptOutput->Lines->Add(UnicodeString(ErrMsgBuffer));
			return false;
		}

		UnicodeString szPath;
		if(HidePathsMenu->Checked){
			szPath = "";
		} else {
			szPath = ExtractFilePath(item->Caption);
		}
		item->SubItems->Strings[1] = szPath + UnicodeString(ModifiedFileName);

		// Rename the file or the directory
		if(!Simulate){
			// Store the original and modified file name
			AnsiString szOriginal = item->Caption;
			AnsiString szModified = szOriginalFilePath + AnsiString(ModifiedFileName);

			// Make sure the original file still exist
			if(FileExists(szOriginal)){

				// Warn the user if the modified file name already exist
				if(FileExists(szModified)){
					char Msg[256];
					ZeroMemory(Msg, 256);
					sprintf(Msg, "The file \"%s\" already exist, replace it?", szModified.c_str());
					if(MessageBoxA(Handle, Msg, "Warning!", MB_YESNOCANCEL) != ID_YES)
						continue;
				}

				// Rename the files here
				RenameFile(szOriginal, szModified);
			}
		}
	}

	return true;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::TestScriptMenuClick(TObject *Sender)
{
	RunScript(true);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ExecuteScriptMenuClick(TObject *Sender)
{
	char Msg[256];
	ZeroMemory(&Msg, 256);
	sprintf(Msg, "This will rename %d files. Continue?", ListViewFilesList->Items->Count);

	if(MessageBoxA(Handle, Msg, "Warning", MB_YESNOCANCEL) == ID_YES){
		RunScript(false);
	}
}


