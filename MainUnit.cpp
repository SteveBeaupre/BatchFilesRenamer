//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "MainUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;
CScriptEngine ScriptEngine;
UnicodeString AppDir;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::LoadSettings()
{
	bool res = false;
	#ifndef DEFAULT_TO_PYTHON
	bool ScriptLanguage = false;
	#else
	bool ScriptLanguage = true;
	#endif
	bool HidePaths = false;

	AnsiString FileName = AppDir + "Settings.set";

	CFileIO f;
	if(f.OpenForReading(FileName.c_str())){

		f.Read(&HidePaths, sizeof(bool));
		f.Read(&ScriptLanguage, sizeof(bool));

		f.Close();
		res = true;
	}

	HidePathsMenu->Checked = HidePaths;
	PythonMenu->Checked = ScriptLanguage;
	LuaMenu->Checked = !ScriptLanguage;

	return res;
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::SaveSettings()
{
	bool HidePaths = HidePathsMenu->Checked;
	bool ScriptLanguage = PythonMenu->Checked;

	AnsiString FileName = AppDir + "Settings.set";

	CFileIO f;
	if(f.OpenForWriting(FileName.c_str())){

		f.Write(&HidePaths, sizeof(bool));
		f.Write(&ScriptLanguage, sizeof(bool));

		f.Close();
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::InitializeUI()
{
	// ***********************************
	// Uncomment this for older look
	// ***********************************
	/*ListViewFilesList->Align = alClient;
	RichEditScriptText->Align = alClient;
	RichEditScriptOutput->Align = alClient;
	ScriptPanel->Align = alClient;
	ScriptPanel->SendToBack();*/

	ScriptPanel->Color = clBtnFace;
	ScriptTextPanel->Color = clBtnFace;
	ScriptOutputPanel->Color = clBtnFace;
	HidePathsMenu->OnClick(this);
	UpdateLeftStatusbarMsg();
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
	AppDir = ExtractFilePath(Application->ExeName);

	#ifdef RUN_FROM_IDE
	AppDir = ExpandFileName(AppDir + "\\..\\..");
	AppDir += "\\";
	//SetCurrentDirectory(AppDir.c_str());
	#endif

	LuaPlugin    = AppDir + "LuaPlugin.dll";
	PythonPlugin = AppDir + "PythonPlugin.dll";

	LoadSettings();

	DragAcceptFiles(Handle, TRUE);
	InitializeUI();

	ScriptEngine.Load(PythonMenu->Checked ? PythonPlugin.c_str() : LuaPlugin.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	ScriptEngine.Free();
	SaveSettings();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::CloseMenuClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::TabControlChange(TObject *Sender)
{
	//FilesListPanel->Visible = TabControl->TabIndex == 0;
	//ScriptPanel->Visible = TabControl->TabIndex == 1;
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
void __fastcall TMainForm::ClearSelectedEntries()
{
	ListViewFilesList->DeleteSelected();
	UpdateLeftStatusbarMsg();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ListViewFilesListKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == VK_DELETE)
		ClearSelectedEntries();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::WndProc(Messages::TMessage &Message)
{
	switch(Message.Msg)
	{
	case WM_DROPFILES:

		HDROP hDrop = (HDROP)Message.WParam;
		UINT NumFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, NULL);

		for(UINT i = 0; i < NumFiles; i++){

			CRawBuffer FileName(MAX_PATH);
			DragQueryFileA(hDrop, i, FileName.GetStrBuffer(), MAX_PATH);

			AnsiString fName = FileName.GetStrBuffer();

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
void __fastcall TMainForm::UpdateLeftStatusbarMsg()
{
	int NumFiles = ListViewFilesList->Items->Count;

	CRawBuffer Msg(256);
	sprintf(Msg.GetStrBuffer(), "%d file%s to rename.", NumFiles, NumFiles < 1 ? "" : "s");
	StatusBar->Panels->Items[0]->Text = Msg.GetStrBuffer();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::UpdateRightStatusbarMsg(int NumRenamed, int TotalItems)
{
	CRawBuffer Msg(256);
	sprintf(Msg.GetStrBuffer(), "%d file%s renamed.", NumRenamed, NumRenamed < 1 ? "" : "s");
	StatusBar->Panels->Items[1]->Text = Msg.GetStrBuffer();
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

	UpdateLeftStatusbarMsg();
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

	UpdateLeftStatusbarMsg();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ClearListMenuClick(TObject *Sender)
{
	ListViewFilesList->Clear();
	UpdateLeftStatusbarMsg();
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

	LuaMenu->Checked = pmi->Tag == 0;
	PythonMenu->Checked = pmi->Tag == 1;
	ScriptEngine.Free();
	ScriptEngine.Load(pmi->Tag == 0 ? LuaPlugin.c_str() : PythonPlugin.c_str());
	RichEditScriptText->Lines->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::LoadScriptMenuClick(TObject *Sender)
{
	OpenDialog->Options.Clear();
	OpenDialog->Options << ofHideReadOnly << ofEnableSizing;
	OpenDialog->Filter = PythonMenu->Checked ? "Python files (*.py*)|*.PY*" : "Lua files (*.lua*)|*.LUA*";

	if(OpenDialog->Execute()){
		RichEditScriptText->Lines->LoadFromFile(OpenDialog->FileName);
	}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SaveScriptMenuClick(TObject *Sender)
{
	SaveDialog->Options.Clear();
	SaveDialog->Options << ofOverwritePrompt << ofHideReadOnly << ofEnableSizing;
	SaveDialog->Filter = PythonMenu->Checked ? "Python files (*.py*)|*.PY*" : "Lua files (*.lua*)|*.LUA*";
	SaveDialog->DefaultExt = PythonMenu->Checked ? "Py" : "lua";

	if(SaveDialog->Execute())
		RichEditScriptText->Lines->SaveToFile(SaveDialog->FileName);
}
//---------------------------------------------------------------------------
bool __fastcall TMainForm::RunScript(bool Simulate)
{
	// Store the nuber of items
	int NumItems = ListViewFilesList->Items->Count;
	if(NumItems < 1){
		ShowMessage("Nothing to rename.");
		return false;
	}

	// Clear the output window
	RichEditScriptOutput->Lines->Clear();

	// Load the script
	AnsiString s = RichEditScriptText->Text.c_str();
	if(!ScriptEngine.LoadScript(s.c_str())){
		ShowMessage("Unable to load script.");
		return false;
	}

	// Output some pre-defined globals variables from the script
	RichEditScriptOutput->Lines->Add("Language Name: " + UnicodeString(ScriptEngine.GetLanguageName()));
	RichEditScriptOutput->Lines->Add("Script Name: " + UnicodeString(ScriptEngine.GetScriptName()));
	RichEditScriptOutput->Lines->Add("Initial Directory Name: " + UnicodeString(ScriptEngine.GetInitialDirectory()));
	RichEditScriptOutput->Lines->Add("");

	// Store the nuber of file renamed
	int NumFilesRenamed = 0;

	// For all items...
	for(int i = 0; i < NumItems; i++){

		// Text Buffers
		CRawBuffer ErrMsgBuffer(1024);
		CRawBuffer ModifiedFileName(MAX_PATH);

		// Store the original file name
		TListItem *item = ListViewFilesList->Items->Item[i];
		AnsiString szOriginalFileName = ExtractFileName(item->Caption);
		AnsiString szOriginalFilePath = ExtractFilePath(item->Caption);

		// Generate the modified name by calling the script's Convert() function
		bool res = ScriptEngine.Convert(szOriginalFileName.c_str(), ModifiedFileName.GetBuffer(), ErrMsgBuffer.GetBuffer());

		// Return false on error
		if(!res || ErrMsgBuffer.GetStrBuffer() != NULL){
			RichEditScriptOutput->Lines->Add(UnicodeString(ErrMsgBuffer.GetStrBuffer()));
			return false;
		}

		// Do we show the path or not?
		UnicodeString szPath;
		if(HidePathsMenu->Checked){
			szPath = "";
		} else {
			szPath = ExtractFilePath(item->Caption);
		}
		item->SubItems->Strings[1] = szPath + UnicodeString(ModifiedFileName.GetStrBuffer());

		// Rename the file or the directory
		if(!Simulate){
			// Store the original and modified file name
			AnsiString szOriginal = item->Caption;
			AnsiString szModified = szOriginalFilePath + AnsiString(ModifiedFileName.GetStrBuffer());

			// Make sure the original file still exist
			if(FileExists(szOriginal)){

				// Warn the user if the modified file name already exist
				if(FileExists(szModified)){
					CRawBuffer Msg(256);
					sprintf(Msg.GetStrBuffer(), "The file \"%s\" already exist, replace it?", szModified.c_str());
					if(MessageBoxA(Handle, Msg.GetStrBuffer(), "Warning!", MB_YESNOCANCEL) != ID_YES)
						continue;
				}

				// Rename the files here
				if(RenameFile(szOriginal, szModified))
					NumFilesRenamed++;
			}
		}
	}

	UpdateRightStatusbarMsg(NumFilesRenamed, NumItems);

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
	int NumItems = ListViewFilesList->Items->Count;
	if(NumItems < 1)
		return;

	CRawBuffer Msg(256);
	sprintf(Msg.GetStrBuffer(), "This will rename %d files. Continue?", NumItems);

	if(MessageBoxA(Handle, Msg.GetStrBuffer(), "Warning", MB_YESNOCANCEL) == ID_YES)
		RunScript(false);
}
//---------------------------------------------------------------------------


