'Convert .doc or .docx to .pdf files
Set fso		= CreateObject("Scripting.FileSystemObject")

If WScript.Arguments.Count > 0 Then
	vsdPath			= fso.GetAbsolutePathName(WScript.Arguments(0))
	page_name		= ""
	
	If WScript.Arguments.Count > 1 Then
		page_name	= WScript.Arguments(1)
		svgPath		= fso.GetFileName(vsdPath) & "." & page_name & ".svg"
	else
		svgPath		= fso.GetFileName(vsdPath) & ".svg"
	End If
	
	svgPath = Replace(svgPath, " ", "_")
	svgPath	= fso.GetParentFolderName(vsdPath) & "\" & svgPath
	
	
	'SVG 파일이 잠겨 있는지 채크
	If fso.FileExists(svgPath) Then
		fso.DeleteFile(svgPath)
		If fso.FileExists(svgPath) Then
			Wscript.Echo "*E: SVG file is locked!"
			WScript.Quit(1)
		End If
	End If
	
	
	If LCase(Right(vsdPath, 4)) = ".vsd" Or LCase(Right(vsdPath, 5)) = ".vsdx" Then
		On Error Resume Next	'오류 강제 처리

		Set objVisio			= CreateObject("Visio.InvisibleApp")

		If Err.Number <> 0 Then
			Wscript.Echo "*E: Microsoft Visio is not installed."
			Err.Clear
			WScript.Quit(1)
		End If

		Set objDraw = objVisio.documents.open(vsdPath)

		'SVG 로 저장
		If page_name = "" Then
			'첫 페이지
			objDraw.Pages(1).Export (svgPath)
		Else
			'페이지 찾기
			bFoundPage	= False
			For Each page in objDraw.Pages
				If page.Name = page_name Then
					objDraw.ActivePage	= page
					page.Export (svgPath)
					bFoundPage	= True
					Exit For
				End If
            Next
			
			If bFoundPage = False Then
				Wscript.Echo "*E: Visio('" & vsdPath & "') page('" & page_name & "') is not found."
			End If
		End If
		
		objDraw.Activate	'alternate wait for page export done!
		
		objDraw.Close 0
		set objDraw		= Nothing
		objVisio.Quit
		Set objVisio	= Nothing
	Else
		Wscript.Echo "*E: Not a Visio file."
	End If
Else
	Wscript.Echo "Usage : vsd2svg  source_vsd_file [page_name]"
End If

Set fso		= Nothing
