'Save .doc or .docx to some extra files

Set fso = CreateObject("Scripting.FileSystemObject")
If WScript.Arguments.Count = 2 Then
	docPath			= fso.GetAbsolutePathName(WScript.Arguments(0))
	output_format	= WScript.Arguments(1)
	output_id		= 8
	save_Path		= fso.GetParentFolderName(docPath) & "\" & fso.GetBaseName(docpath) & "." & output_format
	
	If output_format = "html" Then
		output_id	= 8
	ElseIf output_format = "xml" Then
		output_id	= 12
	ElseIf output_format = "rtf" Then
		output_id	= 6
	ElseIf output_format = "odt" Then
		output_id	= 23
	ElseIf output_format = "txt" Then
		output_id	= 7
	Else
		Wscript.Echo "*E: Output format '" & output_format & "' is not supported!"
		Wscript.Echo "    (supported output format : html, xml, rtf, odt, txt)"
		WScript.Quit(1)
	End If
	
	'타겟 파일이 잠겨 있는지 채크
	If fso.FileExists(save_Path) Then
		fso.DeleteFile(save_Path)
		If fso.FileExists(save_Path) Then
			Wscript.Echo "*E: Target '" & output_format & "' file(" & save_Path & ") is locked!"
			WScript.Quit(1)
		End If
	End If
	
	If LCase(Right(docPath, 4)) = ".doc" Or LCase(Right(docPath, 5)) = ".docx" Then
		On Error Resume Next	'오류 강제 처리
		Set objWord				= CreateObject("Word.Application")
		
		If Err.Number <> 0 Then
			Wscript.Echo "*E: Microsoft Word is not installed."
			Err.Clear
			WScript.Quit(1)
		End If
		
		objWord.Visible			= False
		objWord.DisplayAlerts	= 0
		objWord.OMathAutoCorrect.UseOutsideOMath	= True
		
		Set objDoc = objWord.documents.open(docPath)
		
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

		'HTML 로 저장
		objDoc.SaveAs2 save_Path, output_id
	Else
		Wscript.Echo "*E: Not a Word file."
	End If

	If (objDoc.Saved = False) Then
		objDoc.Save
	End If
	
	objDoc.Activate	'alternate wait for save done!

	objDoc.Close 0	' close without saving
	Set objDoc		= Nothing
	objWord.Quit
	Set objWord		= Nothing
Else
	Wscript.Echo "Usage : doc2save  source_doc_file save_as_format"
	Wscript.Echo "        (supported output format : html, xml, rtf, odt, txt)"
End If

Set fso		= Nothing
