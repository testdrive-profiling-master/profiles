'Convert .doc or .docx to .pdf files
Set fso		= CreateObject("Scripting.FileSystemObject")

If WScript.Arguments.Count > 0 Then
	vsdPath			= fso.GetAbsolutePathName(WScript.Arguments(0))
	page_name		= ""
	
	If WScript.Arguments.Count > 1 Then
		page_name	= WScript.Arguments(1)
		svgPath		= fso.GetParentFolderName(vsdPath) & "\" & fso.GetFileName(vsdPath) & "." & page_name & ".svg"
	else
		svgPath		= fso.GetParentFolderName(vsdPath) & "\" & fso.GetFileName(vsdPath) & ".svg"
	End If
	
	'SVG 파일이 잠겨 있는지 채크
	If fso.FileExists(svgPath) Then
		fso.DeleteFile(svgPath)
		If fso.FileExists(svgPath) Then
			Wscript.Echo "SVG file is locked!"
			WScript.Quit(1)
		End If
	End If
	
	
	If LCase(Right(vsdPath, 4)) = ".vsd" Or LCase(Right(vsdPath, 5)) = ".vsdx" Then
		On Error Resume Next	'오류 강제 처리

		Set objVisio			= CreateObject("Visio.InvisibleApp")

		If Err.Number <> 0 Then
			Wscript.Echo "Microsoft Visio is not installed."
			Err.Clear
			WScript.Quit(1)
		End If
		
		objVisio.Visible		= False
		
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
					page.Export (svgPath)
					bFoundPage	= True
					Exit For
				End If
            Next
			
			If bFoundPage = False Then
				Wscript.Echo "*E: Visio('" & vsdPath & "') page('" & page_name & "') is not found."
			End If
		End If
		
		objVisio.Quit
		objDraw			= NoThing
		Set objVisio	= NoThing
	Else
		Wscript.Echo "Not a Visio file."
	End If
Else
	Wscript.Echo "Usage : vsd2svg  source_vsd_file [page_name]"
End If

Set fso		= NoThing
