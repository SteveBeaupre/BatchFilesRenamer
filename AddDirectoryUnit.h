//---------------------------------------------------------------------------

#ifndef AddDirectoryUnitH
#define AddDirectoryUnitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.FileCtrl.hpp>
//---------------------------------------------------------------------------
class TAddDirectoryForm : public TForm
{
__published:	// IDE-managed Components
	TDirectoryListBox *DirectoryListBox;
	TDriveComboBox *DriveComboBox;
	TCheckBox *CheckBoxRecurse;
	TButton *ButtonCancel;
	TButton *ButtonOk;
private:	// User declarations
public:		// User declarations
	__fastcall TAddDirectoryForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAddDirectoryForm *AddDirectoryForm;
//---------------------------------------------------------------------------
#endif
