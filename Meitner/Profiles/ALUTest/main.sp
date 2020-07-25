system.document.add "ALU Test" {
	chart("Chart", 0, 0, 840, 480){
		Axis(Standard, Left){
			//SetMinMax(0,100);
			SetAutomatic();
			SetTextColor(255,0,0);
			//SetAxisColor(128,128,128);
			Label.SetText("Result");
			Label.SetColor(0,0,255);
			Grid.SetColor(200,200,200);
			Grid.SetVisible(0);
		}
		
		Axis(Standard, Right){
			SetMinMax(0,16);
			//SetAutomatic();
			SetZoomLimit(1);
			SetTextColor(255,0,0);
			//SetAxisColor(128,128,128);
			Label.SetText("Unit in the last place (ulps) : bit error");
			Label.SetColor(0,0,255);
			//Grid.SetColor(200,200,200);
			//Grid.SetVisible(0);
		}
		
		Axis(Standard, Bottom){
			//SetMinMax(0,100);
			SetAutomatic();
			SetTextColor(255,0,0);
			//SetAxisColor(128,128,128);
			Label.SetText("input");
			Label.SetColor(0,0,255);
			Grid.SetColor(200,200,200);
		}

		Serie(Line,0,1){
			SetName("Error (ulps)");
			//SetWidth(2);
			//SetSmooth(1);
		}
		
		Serie(Line){
			SetName("Correct value");
			SetWidth(4);
			//SetSmooth(1);
		}
		
		Serie(Line){
			SetName("ALU result");
			SetWidth(2);
			//SetSmooth(1);
		}
			
		Legend{
			SetVisible(1);
			EnableShadow(1);
			SetHorizontalMode(1);
			DockLegend(Bottom);
		}
		//EnablePan(0);
		//EnableZoom(0);
	}
	
	SetProgram("ALUTest.dll");
}
