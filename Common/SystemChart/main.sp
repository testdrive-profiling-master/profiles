system.document.add "SystemChart" {
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
			SetMinMax(-2,102);
			//SetAutomatic();
			SetZoomLimit(1);
			SetTextColor(255,0,0);
			//SetAxisColor(128,128,128);
			Label.SetText("Ratio (%)");
			Label.SetColor(0,0,255);
			Grid.SetColor(200,200,200);
			//Grid.SetVisible(0);
		}
		
		Axis(Standard, Bottom){
			//SetMinMax(0,100);
			SetAutomatic();
			SetTextColor(255,0,0);
			//SetAxisColor(128,128,128);
			Label.SetText("Time (¥ìs)");
			Label.SetColor(0,0,255);
			Grid.SetColor(200,200,200);
		}

		Serie(Line,0,1){
			SetName("Chart is ready");
			//SetWidth(2);
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
	
	SetProgram("SystemChart.dll");
}
