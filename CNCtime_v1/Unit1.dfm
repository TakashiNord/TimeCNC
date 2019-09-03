object Form1: TForm1
  Left = 299
  Top = 203
  BorderStyle = bsSingle
  Caption = 'Подсчет времени УП (Indramat , Н33....)'
  ClientHeight = 613
  ClientWidth = 807
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDefault
  Visible = True
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 584
    Top = 152
    Width = 150
    Height = 13
    Caption = 'Ускоренное значение подачи'
  end
  object Label2: TLabel
    Left = 584
    Top = 192
    Width = 152
    Height = 13
    Caption = 'Ускорение................................'
  end
  object DirectoryListBox1: TDirectoryListBox
    Left = 8
    Top = 48
    Width = 265
    Height = 385
    FileList = FileListBox1
    ItemHeight = 16
    TabOrder = 0
  end
  object DriveComboBox1: TDriveComboBox
    Left = 8
    Top = 8
    Width = 217
    Height = 19
    DirList = DirectoryListBox1
    TabOrder = 1
  end
  object FileListBox1: TFileListBox
    Left = 288
    Top = 48
    Width = 289
    Height = 345
    FileEdit = EditFile
    ItemHeight = 16
    Mask = '*.dat'
    MultiSelect = True
    ShowGlyphs = True
    TabOrder = 2
    OnChange = FileListBox1Change
  end
  object Button1: TButton
    Left = 640
    Top = 232
    Width = 161
    Height = 41
    Caption = 'Запуск подсчета времени'
    TabOrder = 3
    OnClick = Button1Click
  end
  object EditRapid: TEdit
    Left = 744
    Top = 152
    Width = 57
    Height = 21
    TabOrder = 4
    Text = '10000.0'
    OnChange = EditRapidChange
  end
  object EditFile: TEdit
    Left = 288
    Top = 408
    Width = 289
    Height = 21
    ReadOnly = True
    TabOrder = 5
    Text = '*.dat'
  end
  object FilterComboBox1: TFilterComboBox
    Left = 288
    Top = 8
    Width = 289
    Height = 21
    FileList = FileListBox1
    Filter = 
      'Indramat files (*.dat)|*.dat|Атака - Н33 files (*.up)|*.up|PTP f' +
      'iles (*.ptp)|*.ptp|All files (*.*)|*.*'
    TabOrder = 6
    OnChange = FilterComboBox1Change
  end
  object RadioGroup1: TRadioGroup
    Left = 584
    Top = 24
    Width = 217
    Height = 113
    Caption = 'Система'
    TabOrder = 7
  end
  object MTC200: TRadioButton
    Left = 608
    Top = 48
    Width = 161
    Height = 25
    Caption = 'MTC200 (Indramat)'
    Checked = True
    TabOrder = 8
    TabStop = True
    OnClick = MTC200Click
  end
  object H33: TRadioButton
    Left = 608
    Top = 88
    Width = 177
    Height = 33
    Caption = 'H33 , Атака , ПФСТ-1600'
    TabOrder = 9
    OnClick = H33Click
  end
  object acc: TEdit
    Left = 744
    Top = 184
    Width = 57
    Height = 21
    TabOrder = 10
    Text = '35.0'
    OnChange = accChange
  end
  object Memo1: TMemo
    Left = 8
    Top = 448
    Width = 793
    Height = 153
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 11
  end
  object CheckBox1: TCheckBox
    Left = 664
    Top = 408
    Width = 137
    Height = 33
    Caption = 'Показывать файл .....'
    TabOrder = 12
    OnClick = CheckBox1Click
  end
end
