using System.Windows.Forms;
using Telerik.WinControls;
using Telerik.WinControls.UI;
using Telerik.WinControls.UI.Docking;

namespace TechSun.Theme
{
    public class KRadTheme
    {
        public const string Office2013Light = "Office2013Light";
        public const string Windows8 = "Windows8";
        public const string Office2010Silver = "Office2010Silver";
        // public const string VisualStudio2012Light = "VisualStudio2012Light";
    }

    public class ThemeChangeable : UserControl
    {
        public static void ApplyTheme(ControlCollection oControlColl)
        {
            foreach (Control oCtrl in oControlColl)
            {
                if (oCtrl is RadGroupBox)
                {
                    RadGroupBox oRadGroupBox = oCtrl as RadGroupBox;
                    oRadGroupBox.ThemeName = KRadTheme.Office2010Silver;
                    ApplyTheme(oRadGroupBox.Controls);
                }
                else if (oCtrl is DocumentTabStrip)
                {
                    DocumentTabStrip oDocumentTabStrip = oCtrl as DocumentTabStrip;
                    oDocumentTabStrip.ThemeName = KRadTheme.Office2010Silver;
                }
                else if (oCtrl is FlowLayoutPanel)
                {
                    FlowLayoutPanel oFlowLayoutPanel = oCtrl as FlowLayoutPanel;
                    ApplyTheme(oFlowLayoutPanel.Controls);
                }
                else if (oCtrl is TabControl)
                {
                    TabControl oTabControl = oCtrl as TabControl;
                    ApplyTheme(oTabControl.Controls);
                }
                else if (oCtrl is TabPage)
                {
                    TabPage oTabPage = oCtrl as TabPage;
                    ApplyTheme(oTabPage.Controls);
                }
                else if (oCtrl is RadPageView)
                {
                    RadPageView oRadPageView = oCtrl as RadPageView;
                    oRadPageView.ThemeName = KRadTheme.Office2010Silver;
                    ApplyTheme(oRadPageView.Controls);
                }
                else if (oCtrl is RadPageViewPage)
                {
                    RadPageViewPage oRadPageViewPage = oCtrl as RadPageViewPage;
                    ApplyTheme(oRadPageViewPage.Controls);
                }
                else if (oCtrl is SplitPanel)
                {
                    SplitPanel oSplitPanel = oCtrl as SplitPanel;
                    oSplitPanel.ThemeName = KRadTheme.Office2010Silver;
                    ApplyTheme(oSplitPanel.Controls);
                }
                else if (oCtrl is RadSplitContainer)
                {
                    RadSplitContainer oSplitContainer = oCtrl as RadSplitContainer;
                    oSplitContainer.ThemeName = KRadTheme.Office2010Silver;
                    ApplyTheme(oSplitContainer.Controls);
                }
                else if (oCtrl is RadTreeView)
                {
                    RadTreeView oTreeView = oCtrl as RadTreeView;
                    oTreeView.ThemeName = KRadTheme.Windows8;
                }
                else if (oCtrl is RadGridView)
                {
                    RadGridView oGridView = oCtrl as RadGridView;
                    oGridView.ThemeName = KRadTheme.Office2010Silver;

                    if (oCtrl.Parent is ToolWindow)
                    {
                        ToolWindow toolwindow = oCtrl.Parent as ToolWindow;
                        if ("Connection" == toolwindow.Text.Trim() || "Status" == toolwindow.Text.Trim())
                        {
                            oGridView.ThemeName = KRadTheme.Windows8;
                        }
                    }
                    else if (oCtrl.Parent is UserControl) // EDB Status grid를 위한
                    {
                        if ("ucWorkingStatus" == oCtrl.Parent.Name)
                        {
                            oGridView.ThemeName = KRadTheme.Windows8;
                        }
                    }
                }
                else if (oCtrl is RadListView)
                {
                    RadListView oListView = oCtrl as RadListView;
                    oListView.ThemeName = KRadTheme.Office2010Silver;
                }
                else if (oCtrl is RadDropDownList)
                {
                    RadDropDownList oRadDropDownList = oCtrl as RadDropDownList;
                    oRadDropDownList.ThemeName = KRadTheme.Windows8;
                    oRadDropDownList.ListElement.Font = new System.Drawing.Font("Tahoma", 8.25f);
                    oRadDropDownList.ListElement.ForeColor = System.Drawing.Color.Black;
                    oRadDropDownList.ForeColor = System.Drawing.Color.Black;
                }
                else if (oCtrl is Telerik.WinControls.UI.Docking.RadDock)
                {
                    Telerik.WinControls.UI.Docking.RadDock oRadCtrl = oCtrl as Telerik.WinControls.UI.Docking.RadDock;
                    oRadCtrl.ThemeName = KRadTheme.Office2010Silver;
                }
                else if (oCtrl is RadControl)
                {
                    RadControl oRadCtrl = oCtrl as RadControl;
                    oRadCtrl.ThemeName = KRadTheme.Windows8;

                    if (!(oCtrl is RadLabel))
                    {
                        oRadCtrl.Font = new System.Drawing.Font("Tahoma", 8.25f);
                    }
                }
                else if (oCtrl is UserControl)
                {
                    ApplyTheme((oCtrl as UserControl).Controls);
                }
            }
        }
    }
}
