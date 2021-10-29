Function DocToPdf( docInputFile, pdfOutputFile )
  Dim fileSystemObject
  Dim wordApplication
  Dim wordDocument
  Dim wordDocuments
  Dim baseFolder

  Set fileSystemObject = CreateObject("Scripting.FileSystemObject")
  Set wordApplication = CreateObject("Word.Application")
  Set wordDocuments = wordApplication.Documents

  docInputFile = fileSystemObject.GetAbsolutePathName(docInputFile)
  pdfOutputFile = fileSystemObject.GetAbsolutePathName(pdfOutputFile)
  baseFolder = fileSystemObject.GetParentFolderName(docInputFile)

  If Len(pdfOutputFile) = 0 Then
    pdfOutputFile = fileSystemObject.GetBaseName(docInputFile) + ".pdf"
  End If

  If Len(fileSystemObject.GetParentFolderName(pdfOutputFile)) = 0 Then
    pdfOutputFile = baseFolder + "\" + pdfOutputFile
  End If

  Wscript.Echo docInputFile
  Wscript.Echo pdfOutputFile

  ' Disable any potential macros of the word document.
  wordApplication.WordBasic.DisableAutoMacros

  Set wordDocument = wordDocuments.Open(docInputFile)

  ' See http://msdn2.microsoft.com/en-us/library/bb221597.aspx
  ' wordDocument.SaveAs pdfOutputFile, wdFormatPDF
  wordDocument.ExportAsFixedFormat pdfOutputFile, 17

  wordDocument.Close WdDoNotSaveChanges
  wordApplication.Quit WdDoNotSaveChanges

  Set wordApplication = Nothing
  Set fileSystemObject = Nothing
End Function

docInputFile = WScript.Arguments.Item(0)
pdfOutputFile = WScript.Arguments.Item(1)

Call DocToPdf(docInputFile, pdfOutputFile)