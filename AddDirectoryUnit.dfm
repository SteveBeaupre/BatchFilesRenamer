object AddDirectoryForm: TAddDirectoryForm
  Left = 0
  Top = 0
  Caption = 'Add Directory...'
  ClientHeight = 398
  ClientWidth = 271
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    271
    398)
  PixelsPerInch = 96
  TextHeight = 13
  object DirectoryListBox: TDirectoryListBox
    Left = 8
    Top = 32
    Width = 254
    Height = 305
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 0
  end
  object DriveComboBox: TDriveComboBox
    Left = 8
    Top = 343
    Width = 254
    Height = 19
    Anchors = [akLeft, akRight, akBottom]
    DirList = DirectoryListBox
    TabOrder = 1
  end
  object CheckBoxRecurse: TCheckBox
    Left = 8
    Top = 8
    Width = 57
    Height = 17
    Caption = 'Recurse'
    Checked = True
    State = cbChecked
    TabOrder = 2
  end
  object ButtonCancel: TButton
    Left = 188
    Top = 368
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object ButtonOk: TButton
    Left = 107
    Top = 368
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = 'Ok'
    Default = True
    ModalResult = 1
    TabOrder = 4
  end
end
