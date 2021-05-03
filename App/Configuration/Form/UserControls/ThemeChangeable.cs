using System.Windows.Forms;
using Telerik.WinControls;
using Telerik.WinControls.UI;

namespace Configuration
{
	public class ThemeChangeable : UserControlBase
	{
		public static string ControlTheme{get;set;}
		public static string TreeViewTheme{get;set;}
		public static string PopUpTheme{get;set;}
        public static string MainTheme{get;set;}

		public void ApplyTheme(ControlCollection oControlColl)
		{
			if (null == ControlTheme) ControlTheme = "Office2010Silver";
			if (null == TreeViewTheme) TreeViewTheme = "Office2010Blue";
			if (null == PopUpTheme) PopUpTheme = "Windows7";
			if (null == MainTheme) MainTheme = "Office2013Light";

			foreach (Control oCtrl in oControlColl)
			{
				if (oCtrl is RadGroupBox)
				{
					RadGroupBox oRadGroupBox = oCtrl as RadGroupBox;
					oRadGroupBox.ThemeName = ControlTheme;
					ApplyTheme(oRadGroupBox.Controls);
				}
				else if (oCtrl is SplitPanel)
				{
					SplitPanel oSplitPanel = oCtrl as SplitPanel;
					oSplitPanel.ThemeName = ControlTheme;
					ApplyTheme(oSplitPanel.Controls);
				}
				else if (oCtrl is RadTreeView)
				{
					RadTreeView oTreeView = oCtrl as RadTreeView;
					oTreeView.ThemeName = TreeViewTheme;
				}
				if (oCtrl is RadControl)
				{
					RadControl oRadCtrl = oCtrl as RadControl;
					oRadCtrl.ThemeName = ControlTheme;
				}
				else if (oCtrl is UserControl)
				{
					ApplyTheme((oCtrl as UserControl).Controls);
				}
			}
		}
	}
}
