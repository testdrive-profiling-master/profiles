'Convert .doc or .docx to .pdf files

Set fso = CreateObject("Scripting.FileSystemObject")
If WScript.Arguments.Count > 0 Then
	docPath			= fso.GetAbsolutePathName(WScript.Arguments(0))
	pdfPath			= fso.GetParentFolderName(docPath) & "\" & fso.GetBaseName(docpath) & ".pdf"
	watermark_text	= ""
	modify_source	= False
	
	'PDF 파일이 잠겨 있는지 채크
	If fso.FileExists(pdfPath) Then
		fso.DeleteFile(pdfPath)
		If fso.FileExists(pdfPath) Then
			Wscript.Echo "PDF file is locked!"
			WScript.Quit(1)
		End If
	End If
	
	'Water-mark text
	If WScript.Arguments.Count > 1 Then
		watermark_text	= WScript.Arguments(1)
	End If
	
	'Modify source
	If WScript.Arguments.Count > 2 Then
		If WScript.Arguments(2) = "True" Then
			modify_source	= True
		End If
	End If
	
	If LCase(Right(docPath, 4)) = ".doc" Or LCase(Right(docPath, 5)) = ".docx" Then
		On Error Resume Next	'오류 강제 처리
		Set objWord				= CreateObject("Word.Application")
		objWord.Visible			= False
		objWord.DisplayAlerts	= 0
		objWord.OMathAutoCorrect.UseOutsideOMath	= True
		
		If Err.Number <> 0 Then
			Wscript.Echo "Word is not installed."
			Err.Clear
			WScript.Quit(1)
		End If
		
		if modify_source Then
			Set objDoc = objWord.documents.open(docPath)
		Else
			Set objDoc = objWord.documents.open(docPath,, True)	'read-only 로 열기
		End If
		
		'sub 도큐먼트 확장
		If objDoc.Subdocuments.Count >= 1 Then 
			objDoc.Subdocuments.Expanded = True 
		End If
			
		'모든 field 및 목차 갱신
		objDoc.Fields.Update()
		For t = 1 To objDoc.TablesOfContents.Count
			objDoc.TablesOfContents(t).Update()
		Next
		For t = 1 To objDoc.TablesOfFigures.Count
			objDoc.TablesOfFigures(t).Update()
		Next
		
		'모든 수식 갱신
		For t = 1 To objDoc.OMaths.Count
			objDoc.OMaths(t).BuildUp()
		Next

		For Each rngStory In objDoc.StoryRanges
			If Not rngStory Is Nothing Then
				'Update fields directly in story
				rngStory.Fields.Update
				
				If rngStory.StoryType <> wdMainTextStory Then
					'Update fields in shapes and drawing canvases with shapes
					For Each oShape In rngStory.ShapeRange
						With oShape.TextFrame
							If .HasText Then
								.TextRange.Fields.Update
							End If
							
							'In case of a drawing canvas
							'May contain other shapes that may contain fields
							If oShape.Type = msoCanvas Then
								For Each oShape_2 In oShape.CanvasItems
									With oShape_2.TextFrame
										If .HasText Then
											.TextRange.Fields.Update
										End If
									End With
								Next
							End If
							
						End With
					Next
				End If

				'Handle e.g. multiple sections with unlinked headers/footers or linked text boxes
				If rngStory.StoryType <> wdMainTextStory Then
					While Not (rngStory.NextStoryRange Is Nothing)
						Set rngStory = rngStory.NextStoryRange
						rngStory.Fields.Update
					
						'Update fields in shapes and drawing canvases with shapes
						For Each oShape In rngStory.ShapeRange
							With oShape.TextFrame
								If .HasText Then
									.TextRange.Fields.Update
								End If
								
								'In case of a drawing canvas
								'May contain other shapes that may contain fields
								If oShape.Type = msoCanvas Then
									For Each oShape_2 In oShape.CanvasItems
										With oShape_2.TextFrame
											If .HasText Then
												.TextRange.Fields.Update
											End If
										End With
									Next
								End If
								
							End With
						Next
					Wend
				End If
			End If
		Next
		
		
		'water mark 설정
		If watermark_text <> "" Then
			For Each sec in objDoc.Sections
				'sec.Fields.Update()
				For Each hdr In sec.Headers
					Set shp = hdr.Shapes.AddTextEffect(1, watermark_text, "Arial", 1, True, False, 0, 0)
					'hdr.Fields.Update()
					With shp
						.TextEffect.NormalizedHeight = False
						.Line.Visible = False
						.Fill.Visible = True
						.Fill.Solid
						.Fill.ForeColor.RGB = RGB(205, 205, 205)
						.Fill.Transparency = 0.7
						.Rotation = 315
						.LockAspectRatio = True
						.Height = objWord.InchesToPoints(0.80)
						.Width = objWord.InchesToPoints(7.55)
						.WrapFormat.AllowOverlap = True
						.WrapFormat.Side = 3
						.WrapFormat.Type = 3
						.RelativeHorizontalPosition = 0
						.RelativeVerticalPosition = 0
						.Left = -999995
						.Top = -999995
					End With
				Next
			Next
		End If

		'PDF 로 저장
		objDoc.ExportAsFixedFormat pdfPath, 17, False, 0, 0, 1, 1, 0, True, True, 1, True, True, False
		'objDoc.saveas pdfPath, 17

		if modify_source Then
			objDoc.Close
		Else
			objDoc.Close 0	' close without saving
		End If
		'objDoc.Close 0	' close without saving
		
		objWord.Quit
	Else
		Wscript.Echo "Not a Word file."
	End If
	Set objDoc		= NoThing
	Set objWord		= NoThing
Else
	Wscript.Echo "Usage : doc2pdf  source_doc_file  [water_mark_text]"
End If

Set fso		= NoThing