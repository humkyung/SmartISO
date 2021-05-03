using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

using Telerik.WinControls;
using Telerik.WinControls.UI;
using Telerik.WinControls.Themes;
using Telerik.WinControls.Primitives;
using Telerik.WinControls.Data;
using Telerik.WinControls.UI.Docking;


using System.ComponentModel;
using System.Drawing.Design;
using System.Runtime.InteropServices;
using System.Windows.Forms.VisualStyles;
using Telerik.WinControls.Design;
using Telerik.WinControls.Layouts;

namespace TechSun.Theme
{
    // 정두포만을 위한
    public class KRadMultiColumnComboBox : Telerik.WinControls.UI.RadMultiColumnComboBox
    {
        public KRadMultiColumnComboBox()
        {
            this.Initialized += new EventHandler(KRadMultiColumnComboBox_Initialized);
            this.DropDownOpening += new RadPopupOpeningEventHandler(KRadMultiColumnComboBox_DropDownOpening);
        }

        private void KRadMultiColumnComboBox_DropDownOpening(object sender, CancelEventArgs args)
        {
            //System.Diagnostics.Debug.WriteLine("KRadMultiColumnComboBox_DropDownOpening");

            ((Telerik.WinControls.UI.RadMultiColumnComboBox)(sender)).EditorControl.ThemeName = "Office2010Silver";
        }

        private void KRadMultiColumnComboBox_Initialized(object sender, EventArgs e)
        {
            //System.Diagnostics.Debug.WriteLine("KRadMultiColumnComboBox_Initialized");

            this.ThemeName = KRadTheme.Windows8;
            //this.EditorControl.ThemeName = KRadTheme.Office2010Silver;

            ((Telerik.WinControls.Primitives.ArrowPrimitive)(this.GetChildAt(0).GetChildAt(2).GetChildAt(1).GetChildAt(2))).ForeColor = System.Drawing.Color.Transparent;
            this.Font = new Font("Tahoma", 8.25F);
        }
    }

    public class KToolTabStrip : Telerik.WinControls.UI.Docking.ToolTabStrip
    {
        public KToolTabStrip()
        {
            this.Initialized += new EventHandler(KToolTabStrip_Initialized);
        }

        void KToolTabStrip_Initialized(object sender, EventArgs e)
        {
            //System.Diagnostics.Debug.WriteLine("KToolTabStrip_Initialized");
            foreach (TabPanel tp in this.TabPanels)
            {
                tp.ForeColor = Color.Black;
            }
        }

    }

    public class KRadStatusStrip : Telerik.WinControls.UI.RadStatusStrip
    {
        public KRadStatusStrip()
        {
            this.Initialized += new EventHandler(KRadStatusStrip_Initialized);
        }

        void KRadStatusStrip_Initialized(object sender, EventArgs e)
        {
            //System.Diagnostics.Debug.WriteLine("KRadStatusStrip_Initialized");

            this.ThemeName = TechSun.Theme.KRadTheme.Windows8;

            foreach (RadItem item in this.StatusBarElement.Items)
            {
                if (item is CommandBarSeparator)
                {
                    (item as CommandBarSeparator).Margin = new Padding(20, 0, 4, 0);
                }
                else if (item is RadLabelElement)
                {
                    (item as RadLabelElement).ForeColor = Color.Black;
                    (item as RadLabelElement).Font = new Font("Tahoma", 8.25F);
                }
            }
        }

    }

    // TrackBar
    public class KRadTrackBar : Telerik.WinControls.UI.RadTrackBar
    {
        public KRadTrackBar()
        {
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;

            this.Font = new System.Drawing.Font("Tahoma", 8.25F);
            //this.LargeTickFrequency = 1;
            this.Maximum = 40;
            this.Minimum = 1;
            //this.ShowButtons = true;
            this.Size = new System.Drawing.Size(187, 17);
            this.TickStyle = Telerik.WinControls.Enumerations.TickStyles.None;
            this.Value = 10;

            //trackbar
            //((Telerik.WinControls.UI.TrackBarArrowButton)(this.GetChildAt(0).GetChildAt(0))).ShowBorder = false;
            //((Telerik.WinControls.UI.TrackBarArrowButton)(this.GetChildAt(0).GetChildAt(2))).ShowBorder = false;

            ((Telerik.WinControls.UI.RadTrackBarElement)(this.GetChildAt(0))).MinSize = new System.Drawing.Size(100, 0);
            //((Telerik.WinControls.UI.TrackBarArrowButton)(this.GetChildAt(0).GetChildAt(0))).IsVertical = false;
            //((Telerik.WinControls.UI.TrackBarArrowButton)(this.GetChildAt(0).GetChildAt(0))).Image = global::TechSun.Theme.Properties.Resources.smallMinus;
            ((Telerik.WinControls.Primitives.ImagePrimitive)(this.GetChildAt(0).GetChildAt(0).GetChildAt(1).GetChildAt(0))).ImageScaling = Telerik.WinControls.Enumerations.ImageScaling.SizeToFit;
            ((Telerik.WinControls.Primitives.ImagePrimitive)(this.GetChildAt(0).GetChildAt(0).GetChildAt(1).GetChildAt(0))).AutoSize = true;
            //((Telerik.WinControls.UI.TrackBarArrowButton)(this.GetChildAt(0).GetChildAt(2))).IsVertical = false;
            //((Telerik.WinControls.UI.TrackBarArrowButton)(this.GetChildAt(0).GetChildAt(2))).Image = global::TechSun.Theme.Properties.Resources.smallPlus;
            ((Telerik.WinControls.Primitives.ImagePrimitive)(this.GetChildAt(0).GetChildAt(2).GetChildAt(1).GetChildAt(0))).ImageLayout = System.Windows.Forms.ImageLayout.Center;
            ((Telerik.WinControls.Primitives.ImagePrimitive)(this.GetChildAt(0).GetChildAt(2).GetChildAt(1).GetChildAt(0))).ImageScaling = Telerik.WinControls.Enumerations.ImageScaling.SizeToFit;
        }
    }

    // DocumentWindow
    public class KDocumentWindow : Telerik.WinControls.UI.Docking.DocumentWindow
    {
        public KDocumentWindow()
        {
            this.HandleCreated += new EventHandler(KDocumentWindow_HandleCreated);
        }

        private void KDocumentWindow_HandleCreated(object sender, EventArgs e)
        {
            //System.Diagnostics.Debug.WriteLine("KDocumentWindow_HandleCreated");

            this.SettingDesign();
        }

        private void SettingDesign()
        {
            this.TabStrip.TabStripElement.BackColor = Color.FromArgb(0, 39, 94);
            this.TabStrip.TabStripElement.NumberOfColors = 1;
            this.TabStrip.TabStripElement.GradientStyle = GradientStyles.Solid;

            this.TabStrip.BackColor = Color.FromArgb(235, 235, 235);

            this.BackgroundImage = TechSun.Theme.Properties.Resources.no_data;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.DocumentButtons = DocumentStripButtons.None;
            this.BackColor = Color.FromArgb(235, 235, 235);
            
        }
    }

    public enum HomeImage
    {
        IDCS = 1,
        SPPID = 2,
        EDB = 3,
        SP3D = 4,
		SPOOL = 5,
		GEO = 6,
		PDMS = 7,
    }

    // RibbonBar
    public class KRadRibbonBar : Telerik.WinControls.UI.RadRibbonBar
    {
        public KRadRibbonBar()
        {
            this.ThemeName = KRadTheme.Windows8;
            this.Initialized += new EventHandler(KRadRibbonBar_Initialized);
        }

        void Parent_Resize(object sender, EventArgs e)
        {
            if (this.Parent is KRadRibbonForm)
            {
                if ((this.Parent as KRadRibbonForm).WindowState == FormWindowState.Maximized)
                {
                    ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(1))).Image = global::TechSun.Theme.Properties.Resources.fullsize2;

                }
                else if ((this.Parent as KRadRibbonForm).WindowState == FormWindowState.Normal)
                {
                    ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(1))).Image = global::TechSun.Theme.Properties.Resources.fullsize;
                }
            }
        }

        private void KRadRibbonBar_CommandTabSelected(object sender, CommandTabEventArgs args)
        {
            //System.Diagnostics.Debug.WriteLine("KRadRibbonBar_CommandTabSelected");
            foreach (var item in this.RibbonBarElement.TabStripElement.TabItems)
            {
                if (item.Text == args.CommandTab.Text)
                {
                    item.BackColor = Color.FromArgb(255, 255, 255);
                    item.ForeColor = Color.FromArgb(0, 0, 0);
                }
                else
                {
                    item.BackColor = Color.FromArgb(0, 39, 94);
                    item.ForeColor = Color.FromArgb(255, 255, 255);
                }
            }
        }

        private void KRadRibbonBar_Initialized(object sender, EventArgs e)
        {
            //System.Diagnostics.Debug.WriteLine("KRadRibbonBar_Initialized");

            this.SettingDesign();

            this.CommandTabSelected += new CommandTabEventHandler(KRadRibbonBar_CommandTabSelected);
        }

        public void SetHomeImage(HomeImage homeImage)
        {
            Image ihomeimage = null;
            if (homeImage == HomeImage.EDB) ihomeimage = Properties.Resources.ECS;
            else if (homeImage == HomeImage.IDCS) ihomeimage = Properties.Resources.IDCS;
            else if (homeImage == HomeImage.SPPID) ihomeimage = Properties.Resources.SPPID;
            else if (homeImage == HomeImage.SP3D) ihomeimage = Properties.Resources.SP3D;
			else if ( homeImage == HomeImage.SPOOL ) ihomeimage = Properties.Resources.SPOOL;
			else if (homeImage == HomeImage.GEO) ihomeimage = Properties.Resources.GEO;
			else if(homeImage == HomeImage.PDMS) ihomeimage = Properties.Resources.PDMS;

            this.StartButtonImage = ihomeimage;
        }

        private void SettingDesign()
        {
            this.ThemeClassName = "Telerik.WinControls.UI.RadRibbonBar";
            this.ThemeName = TechSun.Theme.KRadTheme.Windows8;

            this.CommandTabSelected += new CommandTabEventHandler(radRibbonBar1_CommandTabSelected);

            ((Telerik.WinControls.UI.RadRibbonBarElement)(this.GetChildAt(0))).ForeColor = System.Drawing.Color.White; // 타이틀바 텍스트
            ((Telerik.WinControls.UI.RibbonBar.RibbonBarCaptionFillPrimitive)(this.GetChildAt(0).GetChildAt(1))).BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(39)))), ((int)(((byte)(94))))); // 타이틀바 배경색
            //((Telerik.WinControls.UI.InnerItemLayoutElement)(this.GetChildAt(0).GetChildAt(2).GetChildAt(0).GetChildAt(2))).BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(39)))), ((int)(((byte)(94))))); // 퀵버튼 배경

            ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(0))).Image = global::TechSun.Theme.Properties.Resources.downsize2;

            ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(1))).Image = global::TechSun.Theme.Properties.Resources.fullsize;
            if (this.Parent is KRadRibbonForm)
            {
                if ((this.Parent as KRadRibbonForm).WindowState == FormWindowState.Maximized)
                {
                    ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(1))).Image = global::TechSun.Theme.Properties.Resources.fullsize2;

                }
                else if ((this.Parent as KRadRibbonForm).WindowState == FormWindowState.Normal)
                {
                    ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(1))).Image = global::TechSun.Theme.Properties.Resources.fullsize;
                }
            }
            
            ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(0))).ImagePrimitive.PositionOffset = new SizeF(5f, 0f);
            ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(1))).ImagePrimitive.PositionOffset = new SizeF(5f, 0f);
            ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(0))).Size = new Size(27, 27);
            ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(1))).Size = new Size(27, 27);
            ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(2))).Size = new Size(27, 27);
            ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(0))).AutoSize = false;
            ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(1))).AutoSize = false;
            ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(2))).AutoSize = false;

            ((Telerik.WinControls.UI.RadImageButtonElement)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(2))).Image = global::TechSun.Theme.Properties.Resources.close;
            ((Telerik.WinControls.Primitives.ImagePrimitive)(this.GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(2).GetChildAt(1).GetChildAt(0))).PositionOffset = new System.Drawing.SizeF(4F, 0F);

            ((Telerik.WinControls.UI.RibbonTabStripElement)(this.GetChildAt(0).GetChildAt(4))).BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(39)))), ((int)(((byte)(94))))); // 탭 우측 접는 부분
            ((Telerik.WinControls.UI.StripViewItemContainer)(this.GetChildAt(0).GetChildAt(4).GetChildAt(0))).BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(39)))), ((int)(((byte)(94)))));// 탭 우측 중간

            this.RibbonBarElement.ApplicationButtonElement.Margin = new Padding(0, 0, 0, 0); // 시작 버튼 Location
            ((Telerik.WinControls.UI.RadPageViewContentAreaElement)(this.GetChildAt(0).GetChildAt(4).GetChildAt(1))).DrawBorder = false; // Tab 하단 보더 지우기

            ((Telerik.WinControls.UI.RadRibbonBarElement)(this.GetChildAt(0))).ForeColor = System.Drawing.Color.White;
            ((Telerik.WinControls.Primitives.ImagePrimitive)(this.GetChildAt(0).GetChildAt(0))).Alignment = System.Drawing.ContentAlignment.MiddleCenter;
            ((Telerik.WinControls.Primitives.ImagePrimitive)(this.GetChildAt(0).GetChildAt(0))).Visibility = Telerik.WinControls.ElementVisibility.Visible;
            ((Telerik.WinControls.UI.RadQuickAccessOverflowButton)(this.GetChildAt(0).GetChildAt(2).GetChildAt(1))).Visibility = Telerik.WinControls.ElementVisibility.Hidden;
            ((Telerik.WinControls.UI.RibbonTabStripElement)(this.GetChildAt(0).GetChildAt(4))).StripButtons = Telerik.WinControls.UI.StripViewButtons.None;
            ((Telerik.WinControls.UI.RibbonTabStripElement)(this.GetChildAt(0).GetChildAt(4))).ShowItemCloseButton = false;
            ((Telerik.WinControls.UI.RibbonTabStripElement)(this.GetChildAt(0).GetChildAt(4))).BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(39)))), ((int)(((byte)(94)))));

            ((Telerik.WinControls.UI.RadApplicationMenuButtonElement)(this.GetChildAt(0).GetChildAt(5))).DisplayStyle = Telerik.WinControls.DisplayStyle.Image;

            //Tab BackColor
            foreach (var item in this.RibbonBarElement.TabStripElement.TabItems)
            {
                if ("Data" == item.Text || "Piping" == item.Text) // 리본바 첫번째 탭 이름으로 설정
                {
                    item.BackColor = Color.FromArgb(255, 255, 255);
                    item.ForeColor = Color.FromArgb(0, 0, 0);
                }
                else
                {
                    item.BackColor = Color.FromArgb(0, 39, 94);
                    item.ForeColor = Color.FromArgb(255, 255, 255);

                }
            }

            // CheckBoxElement
            foreach (RibbonTab tab in this.CommandTabs)
            {
                foreach(RadRibbonBarGroup group in tab.Items)
                {
                    foreach (RadItem item in group.Items)
                    {
                        if (item is RadCheckBoxElement)
                        {
                            RadCheckBoxElement oRadCheckBoxElement = item as RadCheckBoxElement;
                            oRadCheckBoxElement.CheckMarkPrimitive.CheckElement.ForeColor = Color.Black;
                        }
						else if ( item is RadRibbonBarButtonGroup )
						{
							RadRibbonBarButtonGroup oButtonGroup = item as RadRibbonBarButtonGroup;

							foreach ( RadItem buttonGroupitem in oButtonGroup.Items )
							{
								if ( buttonGroupitem is RadCheckBoxElement )
								{
									RadCheckBoxElement oButtonGroupCheckBoxElement = buttonGroupitem as RadCheckBoxElement;
									oButtonGroupCheckBoxElement.CheckMarkPrimitive.CheckElement.ForeColor = Color.Black;
								}
								else if ( buttonGroupitem is RadRibbonBarButtonGroup )
								{
									RadRibbonBarButtonGroup oButtonGroupOfGroup = buttonGroupitem as RadRibbonBarButtonGroup;

									foreach ( RadItem buttonGroupOfGroupitem in oButtonGroupOfGroup.Items )
									{
										if ( buttonGroupOfGroupitem is RadCheckBoxElement )
										{
											RadCheckBoxElement oButtonGroupOfGroupCheckBoxElement = buttonGroupOfGroupitem as RadCheckBoxElement;
											oButtonGroupOfGroupCheckBoxElement.CheckMarkPrimitive.CheckElement.ForeColor = Color.Black;
										}
										else if ( buttonGroupOfGroupitem is RadRibbonBarButtonGroup )
										{
											RadRibbonBarButtonGroup oButtonGroupOfGroupOfGroup = buttonGroupOfGroupitem as RadRibbonBarButtonGroup;

											foreach ( RadItem buttonGroupOfGroupOfGroupitem in oButtonGroupOfGroupOfGroup.Items )
											{
												if ( buttonGroupOfGroupOfGroupitem is RadCheckBoxElement )
												{
													RadCheckBoxElement oButtonGroupOfGroupOfGroupCheckBoxElement = buttonGroupOfGroupOfGroupitem as RadCheckBoxElement;
													oButtonGroupOfGroupOfGroupCheckBoxElement.CheckMarkPrimitive.CheckElement.ForeColor = Color.Black;
												}
											}
										}
									}
								}
							}
						}
                    }
                }
            }

            foreach (RadItem Item in this.QuickAccessToolBarItems)
            {
                if (Item is RadButtonElement)
                {
                    RadButtonElement buttonElement = Item as RadButtonElement;
                    if(null != buttonElement) buttonElement.Margin = new Padding(2, 0, 2, 0);
                }
            }

        }


        // 리본바 탭 선택 시 색상 변경
        private void radRibbonBar1_CommandTabSelected(object sender, CommandTabEventArgs args)
        {
            //System.Diagnostics.Debug.WriteLine("radRibbonBar1_CommandTabSelected");

            foreach (var item in this.RibbonBarElement.TabStripElement.TabItems)
            {
                if (item.Text == args.CommandTab.Text)
                {
                    item.BackColor = Color.FromArgb(255, 255, 255);
                    item.ForeColor = Color.FromArgb(0, 0, 0);
                }
                else
                {
                    item.BackColor = Color.FromArgb(0, 39, 94);
                    item.ForeColor = Color.FromArgb(255, 255, 255);
                }
            }
        }
    }

    //RadRibbonForm
    public class KRadRibbonForm : Telerik.WinControls.UI.RadRibbonForm
    {
        public KRadRibbonForm()
        {
            this.TopMost = true;
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;
            //Form
            this.FormElement.Border.BorderColor = System.Drawing.Color.FromArgb(0, 39, 94); // form border 색
            this.Icon = TechSun.Theme.Properties.Resources.cs_16;
            this.Font = new Font("Tahoma", 8.25F);

            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;

            this.Resize += new EventHandler(KRadRibbonForm_Resize);
			this.Initialized += KRadRibbonForm_Initialized;
			this.Load += KRadRibbonForm_Load;

        }

		private void KRadRibbonForm_Load(object sender, EventArgs e)
		{
            //System.Diagnostics.Debug.WriteLine("KRadRibbonForm_Load");

			this.TopMost = false;
            this.BringToFront(); 
		}

		private void KRadRibbonForm_Initialized(object sender, EventArgs e)
		{
            //System.Diagnostics.Debug.WriteLine("KRadRibbonForm_Initialized");

			//Form
			this.FormElement.Border.BorderColor = System.Drawing.Color.FromArgb(0, 39, 94); // form border 색
			this.Icon = TechSun.Theme.Properties.Resources.cs_16;
			this.Font = new Font("Tahoma", 8.25F);

			this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;

			this.Resize += new EventHandler(KRadRibbonForm_Resize);
			//this.TopMost = false;
		}

        
        void KRadRibbonForm_Resize(object sender, EventArgs e)
        {
            //System.Diagnostics.Debug.WriteLine("KRadRibbonForm_Resize");

            foreach (Control oCtrl in this.Controls)
            {
                if (oCtrl is KRadRibbonBar)
                {
                    if (this.WindowState == FormWindowState.Maximized)
                    {
                        ((Telerik.WinControls.UI.RadImageButtonElement)((oCtrl as KRadRibbonBar).GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(1))).Image = global::TechSun.Theme.Properties.Resources.fullsize2;

                    }
                    else if (this.WindowState == FormWindowState.Normal)
                    {
                        ((Telerik.WinControls.UI.RadImageButtonElement)((oCtrl as KRadRibbonBar).GetChildAt(0).GetChildAt(3).GetChildAt(1).GetChildAt(1))).Image = global::TechSun.Theme.Properties.Resources.fullsize;
                    }
                }
            }
        }

    }

    //RadDock
    public class KRadDock : Telerik.WinControls.UI.Docking.RadDock
    {
        public KRadDock()
        {
            this.ThemeName = TechSun.Theme.KRadTheme.Office2010Silver;
            this.Initialized += new EventHandler(KRadDock_Initialized);

        }

        private void KRadDock_Initialized(object sender, EventArgs e)
        {
            //System.Diagnostics.Debug.WriteLine("KRadDock_Initialized");

            this.ThemeName = TechSun.Theme.KRadTheme.Office2010Silver;

            //this.SplitContainerElement.BorderThickness = new Padding(1);
            
            this.ShowDocumentCloseButton = false;
            this.SettingDesign();
           
            TechSun.Theme.ThemeChangeable.ApplyTheme(this.Controls);

        }

        private void SettingDesign()
        {  
            this.ActiveWindowChanged += new DockWindowEventHandler(radDock1_ActiveWindowChanged);
            this.DockStateChanging += new DockStateChangingEventHandler(KRadDock_DockStateChanging);
            this.FloatingWindowCreated += new FloatingWindowEventHandler(KRadDock_FloatingWindowCreated);
            this.DockStateChanged += new DockWindowEventHandler(KRadDock_DockStateChanged);

            DragDropService service = this.GetService<DragDropService>();
            service.Starting += new StateServiceStartingEventHandler(service_Starting);

            ContextMenuService menuService = this.GetService<ContextMenuService>();
            menuService.ContextMenuDisplaying += menuService_ContextMenuDisplaying;
        }

        private void KRadDock_DockStateChanged(object sender, DockWindowEventArgs e)
        {
            //System.Diagnostics.Debug.WriteLine("KRadDock_DockStateChanged");

            if (null != e.DockWindow.FloatingParent)
            {
                e.DockWindow.FloatingParent.Text = ("PID Viewer").PadLeft(3 + ("PID Viewer").Length);
                ToolTabStrip tooltabstrip = e.DockWindow.Parent as ToolTabStrip;
                tooltabstrip.ThemeName = KRadTheme.Office2010Silver;
                tooltabstrip.Font = new System.Drawing.Font("Tahoma", 8.25F);
                tooltabstrip.TabStripElement.Font = new System.Drawing.Font("Tahoma", 8.25F);
            }
            else // floating에서 dock되는놈 PIDVIEWER
            {
                ToolWindow toolwindow = e.DockWindow as ToolWindow;
                if (toolwindow != null)
                {
                    toolwindow.Focus();
                    toolwindow.Text = toolwindow.Text.PadLeft(3 + toolwindow.Text.Trim().Length);
                    toolwindow.DocumentButtons = DocumentStripButtons.Close;

                    ToolTabStrip tabStrip = toolwindow.TabStrip as ToolTabStrip;

                    if (null != tabStrip)
                    {
                        tabStrip.ForeColor = Color.White;
                        tabStrip.TabStripElement.ForeColor = Color.White;
                        Telerik.WinControls.Primitives.FillPrimitive fill = tabStrip.CaptionElement.FindDescendant<Telerik.WinControls.Primitives.FillPrimitive>();
                        if (fill != null)
                        {
                            fill.NumberOfColors = 1;
                            fill.BackColor = Color.FromArgb(0, 39, 94);
                            fill.GradientStyle = Telerik.WinControls.GradientStyles.Solid;
                            fill.ForeColor = Color.White;
                            fill.Font = new System.Drawing.Font("Tahoma", 8.25F);
                        }
                    }
                }
            }
        }

        private void KRadDock_FloatingWindowCreated(object sender, FloatingWindowEventArgs e)
        {
            //System.Diagnostics.Debug.WriteLine("KRadDock_FloatingWindowCreated");

            e.Window.ThemeName = KRadTheme.Office2010Silver;
            e.Window.FormBorderStyle = FormBorderStyle.Sizable;
            e.Window.FormElement.TitleBar.FillPrimitive.NumberOfColors = 1;
            e.Window.FormElement.TitleBar.FillPrimitive.BackColor = System.Drawing.Color.FromArgb(0, 39, 94);
            e.Window.FormElement.TitleBar.FillPrimitive.ForeColor = System.Drawing.Color.White;
            e.Window.FormElement.TitleBar.FillPrimitive.GradientStyle = Telerik.WinControls.GradientStyles.Solid;
            e.Window.FormElement.TitleBar.TitlePrimitive.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);

            e.Window.ShowIcon = false;
            // 161112 팝업창 최대화버튼 생성 요청 : 오세환 과장님
            //e.Window.MaximizeBox = false;

            e.Window.FormElement.ImageBorder.BackColor = System.Drawing.Color.FromArgb(0, 39, 94);
            e.Window.FormElement.Border.ForeColor = System.Drawing.Color.FromArgb(0, 39, 94);

            e.Window.FormElement.TitleBar.BorderPrimitive.ForeColor = System.Drawing.Color.FromArgb(0, 39, 94);
           
            if (e.Window.ParentForm is FloatingWindow)
            {
                e.Window.ParentForm.Text = ("PID Viewer").PadLeft(3 + ("PID Viewer").Length);
                e.Window.ParentForm.Font = new Font("Tahoma", 8.25F);
            }
        }

        private void KRadDock_DockStateChanging(object sender, DockStateChangingEventArgs e)
        {
            //System.Diagnostics.Debug.WriteLine("KRadDock_DockStateChanging");

            if (e.NewDockState == DockState.Floating)
            {
                bool bDragDropService = true;
                // 161112 기존 PID VIEWER만 제어하던것을 PBS, Connection에 적용하도록 수정(GS Lee)
                if ((e.NewWindow.Text.ToUpper().Contains("PID VIEWER")) || e.NewWindow.Text.ToUpper().Contains("PBS") || e.NewWindow.Text.Trim().ToUpper().Contains("CONNECTION"))
                {
                    bDragDropService = false;
                }

                e.Cancel = bDragDropService;
            }
        }

        void service_Starting(object sender, StateServiceStartingEventArgs e)
        {
            if (e != null)
            {
                bool bDragDropService = true;

                if (e.Context is AutoHideTabStrip)
                {
                    AutoHideTabStrip ItemAutoHide = e.Context as AutoHideTabStrip;
                    if (ItemAutoHide != null)
                    {
                        foreach (var item in ItemAutoHide.Controls)
                        {
                            Telerik.WinControls.UI.Docking.ToolWindow itemToolWindow = item as Telerik.WinControls.UI.Docking.ToolWindow;
                            if (itemToolWindow != null)
                            {
                                if (itemToolWindow.Text.ToUpper().Contains("PID VIEWER"))
                                {
                                    bDragDropService = false;
                                   
                                    break;
                                }
                            }
                        }
                    }
                }
                else if (e.Context is FloatingWindow)
                {
                    FloatingWindow ItemFloatingWindow = e.Context as FloatingWindow;
                    if (ItemFloatingWindow != null)
                    {
                        // 161112 기존 PID VIEWER만 제어하던것을 PBS, Connection에 적용하도록 수정(GS Lee)
                        if (ItemFloatingWindow.Text.ToUpper().Contains("PID VIEWER") || string.IsNullOrEmpty(ItemFloatingWindow.Text.Trim()) || ItemFloatingWindow.Text.ToUpper().Contains("PBS") || ItemFloatingWindow.Text.Trim().ToUpper().Contains("CONNECTION"))
                        {
                            bDragDropService = false;
                        }
                    }
                }
                else if (e.Context is ToolTabStrip)
                {
                    ToolTabStrip ItemToolTabStrip = e.Context as ToolTabStrip;
                    if (ItemToolTabStrip != null)
                    {
                        ItemToolTabStrip.ThemeName = KRadTheme.Office2010Silver;
                        foreach (var item in ItemToolTabStrip.Controls)
                        {
                            Telerik.WinControls.UI.Docking.ToolWindow itemToolWindow = item as Telerik.WinControls.UI.Docking.ToolWindow;
                            if (itemToolWindow != null)
                            {
                                // 161112 기존 PID VIEWER만 제어하던것을 PBS, Connection에 적용하도록 수정(GS Lee)
                                if (itemToolWindow.Text.ToUpper().Contains("PID VIEWER") || itemToolWindow.Text.ToUpper().Contains("PBS") || itemToolWindow.Text.Trim().ToUpper().Contains("CONNECTION"))
                                {
                                    bDragDropService = false;

                                    break;
                                }
                            }
                        }
                    }
                }
                else if (e.Context is ToolWindow)
                {
                    Telerik.WinControls.UI.Docking.ToolWindow itemToolWindow = e.Context as Telerik.WinControls.UI.Docking.ToolWindow;
                    if (itemToolWindow != null)
                    {
                        // 161112 기존 PID VIEWER만 제어하던것을 PBS, Connection에 적용하도록 수정(GS Lee)
                        if (itemToolWindow.Text.ToUpper().Contains("PID VIEWER") || itemToolWindow.Text.ToUpper().Contains("PBS") || itemToolWindow.Text.Trim().ToUpper().Contains("CONNECTION"))
                        {
                            bDragDropService = false;
                        }
                    }
                }
                e.Cancel = bDragDropService;
            }
        }

        // RadDock context Menu 차단 이벤트
        private void menuService_ContextMenuDisplaying(object sender, ContextMenuDisplayingEventArgs e)
        {
            e.Cancel = true;
        }

        // toolwindow Titlebar 배경색 변경
        private void radDock1_ActiveWindowChanged(object sender, DockWindowEventArgs e)
        {
            //System.Diagnostics.Debug.WriteLine("radDock1_ActiveWindowChanged");

            ToolWindow toolwindow = e.DockWindow as ToolWindow;
            if (toolwindow != null)
            {
                toolwindow.Text = toolwindow.Text.PadLeft(3 + toolwindow.Text.Trim().Length);
                toolwindow.DocumentButtons = DocumentStripButtons.Close;
                toolwindow.BackColor = Color.FromArgb(235, 235, 235);
                toolwindow.ToolCaptionButtons = ToolStripCaptionButtons.Close;

                ThemeChangeable.ApplyTheme(toolwindow.Controls);

                ToolTabStrip tabStrip = toolwindow.TabStrip as ToolTabStrip;
                tabStrip.ForeColor = Color.White;
                tabStrip.TabStripElement.ForeColor = Color.Black;
                tabStrip.TabStripElement.Font = new System.Drawing.Font("Tahoma", 8.25F);

                Telerik.WinControls.Primitives.FillPrimitive fill = tabStrip.CaptionElement.FindDescendant<Telerik.WinControls.Primitives.FillPrimitive>();
                if (fill != null)
                {
                    fill.NumberOfColors = 1;
                    fill.BackColor = Color.FromArgb(0, 39, 94);
                    fill.GradientStyle = Telerik.WinControls.GradientStyles.Solid;
                    fill.ForeColor = Color.White;
                }

                foreach (ToolWindow tw in this.DockWindows.ToolWindows)
                {
                    if (tw != toolwindow)
                    {
                        ThemeChangeable.ApplyTheme(tw.Controls);

                        tw.Text = tw.Text.PadLeft(3 + tw.Text.Trim().Length);
                        tw.DocumentButtons = DocumentStripButtons.Close;
                        tw.BackColor = Color.FromArgb(235, 235, 235);
                        tw.ToolCaptionButtons = ToolStripCaptionButtons.Close;

                        tabStrip = tw.TabStrip as ToolTabStrip;
                        if (null != tabStrip)
                        {
                            tabStrip.ForeColor = Color.White;
                            tabStrip.TabStripElement.ForeColor = Color.Black;
                            tabStrip.TabStripElement.Font = new System.Drawing.Font("Tahoma", 8.25F);
                            
                            Telerik.WinControls.Primitives.FillPrimitive fillPrimitive = tabStrip.CaptionElement.FindDescendant<Telerik.WinControls.Primitives.FillPrimitive>();
                            if (fillPrimitive != null)
                            {
                                fillPrimitive.NumberOfColors = 1;
                                fillPrimitive.BackColor = Color.FromArgb(0, 39, 94);
                                fillPrimitive.GradientStyle = Telerik.WinControls.GradientStyles.Solid;
                                fillPrimitive.ForeColor = Color.White;
                            }
                        }
                    }
                }
            }

        }

    }

    // UserControl
    #region KUserControl

    public class KUserControl : UserControl
    {
        /// <summary>
        /// 내 IP 인지 외부 IP 인지 확인 하는 Flag 외부 False 내부 True 
        /// </summary>
        

        public KUserControl()
        {
            this.Load += new EventHandler(KUserControl_Load);
        }

        void KUserControl_Load(object sender, EventArgs e)
        {
            //System.Diagnostics.Debug.WriteLine("KUserControl_Load");

            TechSun.Theme.ThemeChangeable.ApplyTheme(this.Controls);
        }

    }

    #endregion

    #region KRadForm
    //SubForm
    public class KRadForm : Telerik.WinControls.UI.RadForm
    {
        public KRadForm()
        {
            this.Resize += new EventHandler(KRadForm_Resize);
            this.Initialized += KRadForm_Initialized;
            
        }

        void KRadForm_Initialized(object sender, EventArgs e)
        {
            this.ShowIcon = false;
            this.ThemeName = TechSun.Theme.KRadTheme.Windows8;
            this.BackColor = System.Drawing.Color.White;
            this.Text = Text.Trim().PadLeft(3 + Text.Trim().Length);

            this.FormElement.TitleBar.FillPrimitive.BackColor = System.Drawing.Color.FromArgb(0, 39, 94);
            this.FormElement.TitleBar.FillPrimitive.GradientStyle = Telerik.WinControls.GradientStyles.Solid;
            this.FormElement.TitleBar.TitlePrimitive.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);

            this.FormElement.TitleBar.BorderPrimitive.ForeColor = System.Drawing.Color.FromArgb(0, 39, 94);

            this.FormElement.ImageBorder.BackColor = System.Drawing.Color.FromArgb(0, 39, 94);
            this.FormElement.Border.ForeColor = System.Drawing.Color.FromArgb(0, 39, 94);

            this.FormElement.TitleBar.MinimizeButton.Image = global::TechSun.Theme.Properties.Resources.downsize2;
            this.FormElement.TitleBar.MaximizeButton.Image = global::TechSun.Theme.Properties.Resources.fullsize;
            if (this.WindowState == FormWindowState.Maximized)
            {
                this.FormElement.TitleBar.MaximizeButton.Image = global::TechSun.Theme.Properties.Resources.fullsize2;

            }
            else if (this.WindowState == FormWindowState.Normal)
            {
                this.FormElement.TitleBar.MaximizeButton.Image = global::TechSun.Theme.Properties.Resources.fullsize;
            }

            //close button
            this.FormElement.TitleBar.CloseButton.AutoSize = false;
            this.FormElement.TitleBar.CloseButton.Size = new Size(27, 22);
            this.FormElement.TitleBar.CloseButton.Image = global::TechSun.Theme.Properties.Resources.close;
            this.FormElement.TitleBar.CloseButton.ImagePrimitive.PositionOffset = new SizeF(4F, 0F);

            this.FormElement.TitleBar.MaximizeButton.AutoSize = false;
            this.FormElement.TitleBar.MaximizeButton.Size = new Size(27, 22);
            this.FormElement.TitleBar.MaximizeButton.ImagePrimitive.PositionOffset = new SizeF(4F, 0F);

            this.FormElement.TitleBar.MinimizeButton.Size = new Size(27, 22);
            this.FormElement.TitleBar.MinimizeButton.ImagePrimitive.PositionOffset = new SizeF(4F, 0F);
            this.FormElement.TitleBar.MinimizeButton.AutoSize = false;

            TechSun.Theme.ThemeChangeable.ApplyTheme(this.Controls);
        }

        void KRadForm_Resize(object sender, EventArgs e)
        {
            if (this.WindowState == FormWindowState.Maximized)
            {
                this.FormElement.TitleBar.MaximizeButton.Image = global::TechSun.Theme.Properties.Resources.fullsize2;

            }
            else if (this.WindowState == FormWindowState.Normal)
            {
                this.FormElement.TitleBar.MaximizeButton.Image = global::TechSun.Theme.Properties.Resources.fullsize;
            }
        }
    }

    #endregion

    // Button
    //public class KRadButtonElement : RadButtonElement
    //{
    //    public event EventHandler Initialized;
    //    public KRadButtonElement()
    //    {
    //        var roundRectShape1 = new Telerik.WinControls.RoundRectShape();
    //        roundRectShape1.Radius = 7;

    //        this.BorderElement.ForeColor = Color.Black;
    //        this.Shape = roundRectShape1;
    //        this.ShowBorder = true;

    //        if (null != Initialized)
    //        {
    //            Initialized(this, EventArgs.Empty);
    //        }

    //        this.Initialized += new EventHandler(KRadButtonElement_Initialized);
    //    }

    //    private void KRadButtonElement_Initialized(object sender, EventArgs e)
    //    {
    //        var roundRectShape1 = new Telerik.WinControls.RoundRectShape();
    //        roundRectShape1.Radius = 7;

    //        this.BorderElement.ForeColor = Color.Black;
    //        this.Shape = roundRectShape1;
    //        this.ShowBorder = true;
    //    }
    //}

    public class KRadFreeButton : Telerik.WinControls.UI.RadButton
    {
        public const int ButtonHeight = 26;
        public KRadFreeButton()
        {
            this.Initialized += new EventHandler(KRadButton_Initialized);
        }

        void KRadButton_Initialized(object sender, EventArgs e)
        {
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;
			this.Font = new Font("Tahoma", 8.25f);

            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);

            this.MouseEnter += new EventHandler(KRadButton_MouseEnter);
            this.MouseLeave += new EventHandler(KRadButton_MouseLeave);
        }

        void KRadButton_MouseLeave(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
        }

        void KRadButton_MouseEnter(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
            this.ButtonElement.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);

        }
    }

    // Button
    public class KRadToggleButton : Telerik.WinControls.UI.RadToggleButton
    {
        public KRadToggleButton()
        {
            this.Initialized += new EventHandler(KRadToggleButton_Initialized);
        }

        void KRadToggleButton_Initialized(object sender, EventArgs e)
        {
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;
            this.Height = KRadButton.ButtonHeight;
			this.Font = new Font("Tahoma", 8.25f);

            this.ButtonElement.ButtonFillElement.BackColor = Color.White;

            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);

            this.MouseEnter += new EventHandler(KRadToggleButton_MouseEnter);
            this.MouseLeave += new EventHandler(KRadToggleButton_MouseLeave);
        }

        void KRadToggleButton_MouseLeave(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
        }

        void KRadToggleButton_MouseEnter(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
            this.ButtonElement.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);

        }
    }

	public class KRadBorderButton : Telerik.WinControls.UI.RadButton
	{
		public const int ButtonHeight = 26;
		public KRadBorderButton()
		{
			this.Initialized += new EventHandler(KRadButton_Initialized);
		}

		void KRadButton_Initialized(object sender, EventArgs e)
		{
			this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;
			this.Height = KRadButton.ButtonHeight;
			this.Font = new Font("Tahoma", 8.25f);

			this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
			this.ButtonElement.ButtonFillElement.BackColor = Color.White;
			this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
			this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);

			// Border 추가
			this.ButtonElement.BorderElement.ForeColor = Color.Black;

			this.MouseEnter += new EventHandler(KRadButton_MouseEnter);
			this.MouseLeave += new EventHandler(KRadButton_MouseLeave);
		}

		void KRadButton_MouseLeave(object sender, EventArgs e)
		{
			this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
			this.ButtonElement.ButtonFillElement.BackColor = Color.White;
			this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
			this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
		}

		void KRadButton_MouseEnter(object sender, EventArgs e)
		{
			this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
			this.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
			this.ButtonElement.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);
			this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);

		}
	}

    public class KRadButton : Telerik.WinControls.UI.RadButton
    {
        public const int ButtonHeight = 26;
        public KRadButton()
        {
            this.Initialized += new EventHandler(KRadButton_Initialized);
        }

        void KRadButton_Initialized(object sender, EventArgs e)
        {
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;
            this.Height = KRadButton.ButtonHeight;
			this.Font = new Font("Tahoma", 8.25f);

            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);

            this.MouseEnter += new EventHandler(KRadButton_MouseEnter);
            this.MouseLeave += new EventHandler(KRadButton_MouseLeave);
        }

        void KRadButton_MouseLeave(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
        }

        void KRadButton_MouseEnter(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
            this.ButtonElement.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);

        }
    }

    public class KRadLeftImageButton : Telerik.WinControls.UI.RadButton
    {
        public KRadLeftImageButton()
        {
            this.Initialized += new EventHandler(KRadLeftImageButton_Initialized);
            this.Height = KRadButton.ButtonHeight;

            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_prev;
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;

            this.MouseEnter += new EventHandler(KRadButton_MouseEnter);
            this.MouseLeave += new EventHandler(KRadButton_MouseLeave);
        }

        void KRadLeftImageButton_Initialized(object sender, EventArgs e)
        {
            this.Height = KRadButton.ButtonHeight;
			this.Font = new Font("Tahoma", 8.25f);

            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_prev;
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;

            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);

            this.MouseEnter += new EventHandler(KRadButton_MouseEnter);
            this.MouseLeave += new EventHandler(KRadButton_MouseLeave);
        }

        void KRadButton_MouseLeave(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_prev;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
        }

        void KRadButton_MouseEnter(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_prev_on;
            this.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
            this.ButtonElement.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);

        }
    }

    public class KRadFirstImageButton : Telerik.WinControls.UI.RadButton
    {
        public KRadFirstImageButton()
        {
            this.Initialized += new EventHandler(KRadFirstImageButton_Initialized);
            
        }

        void KRadFirstImageButton_Initialized(object sender, EventArgs e)
        {
            this.Height = KRadButton.ButtonHeight;
			this.Font = new Font("Tahoma", 8.25f);

            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_first;
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;

            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);

            this.MouseEnter += new EventHandler(KRadButton_MouseEnter);
            this.MouseLeave += new EventHandler(KRadButton_MouseLeave);
        }

        void KRadButton_MouseLeave(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_first;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
        }

        void KRadButton_MouseEnter(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_first_on;
            this.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
            this.ButtonElement.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);

        }
    }

    public class KRadLastImageButton : Telerik.WinControls.UI.RadButton
    {
        public KRadLastImageButton()
        {
            this.Initialized += new EventHandler(KRadLastImageButton_Initialized);
            
        }

        void KRadLastImageButton_Initialized(object sender, EventArgs e)
        {
            this.Height = KRadButton.ButtonHeight;
			this.Font = new Font("Tahoma", 8.25f);

            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_last;
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;

            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);

            this.MouseEnter += new EventHandler(KRadButton_MouseEnter);
            this.MouseLeave += new EventHandler(KRadButton_MouseLeave);
        }

        void KRadButton_MouseLeave(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_last;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
        }

        void KRadButton_MouseEnter(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_last_on;
            this.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
            this.ButtonElement.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);

        }
    }

    public class KRadRightImageButton : Telerik.WinControls.UI.RadButton
    {
        public KRadRightImageButton()
        {
            this.Initialized += new EventHandler(KRadRightImageButton_Initialized);
            
        }

        void KRadRightImageButton_Initialized(object sender, EventArgs e)
        {
            this.Height = KRadButton.ButtonHeight;
			this.Font = new Font("Tahoma", 8.25f);

            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_next;
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;

            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);

            this.MouseEnter += new EventHandler(KRadButton_MouseEnter);
            this.MouseLeave += new EventHandler(KRadButton_MouseLeave);
        }

        void KRadButton_MouseLeave(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_next;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
        }

        void KRadButton_MouseEnter(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_next_on;
            this.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
            this.ButtonElement.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);

        }
    }

    public class KRadUpImageButton : Telerik.WinControls.UI.RadButton
    {
        public KRadUpImageButton()
        {
            this.Initialized += new EventHandler(KRadUpImageButton_Initialized);
            
        }

        void KRadUpImageButton_Initialized(object sender, EventArgs e)
        {
            this.Height = KRadButton.ButtonHeight;
			this.Font = new Font("Tahoma", 8.25f);

            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_up;
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;

            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);

            this.MouseEnter += new EventHandler(KRadButton_MouseEnter);
            this.MouseLeave += new EventHandler(KRadButton_MouseLeave);
        }

        void KRadButton_MouseLeave(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_up;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
        }

        void KRadButton_MouseEnter(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_up_on;
            this.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
            this.ButtonElement.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);

        }
    }

    public class KRadDownImageButton : Telerik.WinControls.UI.RadButton
    {
        public KRadDownImageButton()
        {
            this.Initialized += new EventHandler(KRadDownImageButton_Initialized);
            
        }

        void KRadDownImageButton_Initialized(object sender, EventArgs e)
        {
            this.Height = KRadButton.ButtonHeight;
			this.Font = new Font("Tahoma", 8.25f);

            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_down;
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;

            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);

            this.MouseEnter += new EventHandler(KRadButton_MouseEnter);
            this.MouseLeave += new EventHandler(KRadButton_MouseLeave);
        }

        void KRadButton_MouseLeave(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_down;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
        }

        void KRadButton_MouseEnter(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.arrow_down_on;
            this.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
            this.ButtonElement.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);

        }
    }

    public class KRadCloseImageButton : Telerik.WinControls.UI.RadButton
    {
        public KRadCloseImageButton()
        {
            this.Initialized += new EventHandler(KRadCloseImageButton_Initialized);
            
        }

        void KRadCloseImageButton_Initialized(object sender, EventArgs e)
        {
            this.Height = KRadButton.ButtonHeight;
			this.Font = new Font("Tahoma", 8.25f);

            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.ico_xdelect;
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;

            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);

            this.MouseEnter += new EventHandler(KRadButton_MouseEnter);
            this.MouseLeave += new EventHandler(KRadButton_MouseLeave);
        }

        void KRadButton_MouseLeave(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.ico_xdelect;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
        }

        void KRadButton_MouseEnter(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.ico_xdelect_on;
            this.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
            this.ButtonElement.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);

        }
    }

    public class KRadPlusImageButton : Telerik.WinControls.UI.RadButton
    {
        public KRadPlusImageButton()
        {
            this.Initialized += new EventHandler(KRadPlusImageButton_Initialized);
           
        }

        void KRadPlusImageButton_Initialized(object sender, EventArgs e)
        {
            this.Height = KRadButton.ButtonHeight;
			this.Font = new Font("Tahoma", 8.25f);

            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.ico_plusadd;
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;

            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);

            this.MouseEnter += new EventHandler(KRadButton_MouseEnter);
            this.MouseLeave += new EventHandler(KRadButton_MouseLeave);
        }

        void KRadButton_MouseLeave(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.ico_plusadd;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
        }

        void KRadButton_MouseEnter(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.ico_plusadd_on;
            this.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
            this.ButtonElement.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);

        }
    }

    public class KRadSaveButton : Telerik.WinControls.UI.RadButton
    {
        public KRadSaveButton()
        {
            this.Initialized += new EventHandler(KRadSaveButton_Initialized);

        }

        void KRadSaveButton_Initialized(object sender, EventArgs e)
        {
            this.Height = KRadButton.ButtonHeight;
            this.Font = new Font("Tahoma", 8.25f);

            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.disk;
            this.ThemeName = TechSun.Theme.KRadTheme.Office2013Light;

            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);

            this.MouseEnter += new EventHandler(KRadButton_MouseEnter);
            this.MouseLeave += new EventHandler(KRadButton_MouseLeave);
        }

        void KRadButton_MouseLeave(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.disk;
            this.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
        }

        void KRadButton_MouseEnter(object sender, EventArgs e)
        {
            this.ButtonElement.ButtonFillElement.NumberOfColors = 1;
            this.ButtonElement.Image = global::TechSun.Theme.Properties.Resources.disk;
            this.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
            this.ButtonElement.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);
            this.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);

        }
    }

    //public class KRadGridView : Telerik.WinControls.UI.RadGridView
    //{
    //    public KRadGridView()
    //    {
    //        this.Initialized += new EventHandler(KRadGridView_Initialized);
    //        this.ViewCellFormatting += new CellFormattingEventHandler(KRadGridView_ViewCellFormatting);
    //    }

    //    void KRadGridView_ViewCellFormatting(object sender, CellFormattingEventArgs e)
    //    {
    //        e.CellElement.NumberOfColors = 1;
    //        e.CellElement.DrawBorder = true;
    //        e.CellElement.BorderWidth = 1;
    //        e.CellElement.GradientStyle = GradientStyles.Solid;

    //        e.CellElement.Enabled = true;

    //        if (e.CellElement.IsMouseOver)
    //        {
    //            if (e.CellElement is GridHeaderCellElement)
    //            {
    //                e.CellElement.BackColor = Color.FromArgb(187, 196, 209);
    //                e.CellElement.ForeColor = Color.Black;
    //                e.CellElement.BorderColor = Color.FromArgb(134, 138, 150);
    //                e.CellElement.Font = new Font("Tahoma", 9.0f);
    //            }
    //        }
    //        else if (e.CellElement.IsCurrent)
    //        {
    //            if (e.CellElement is GridHeaderCellElement)
    //            {
    //                e.CellElement.DrawFill = true;
    //                e.CellElement.BackColor = Color.FromArgb(74, 101, 140);
    //                e.CellElement.ForeColor = Color.White;
    //                e.CellElement.BorderColor = Color.FromArgb(134, 138, 150);
    //                e.CellElement.Font = new Font("Tahoma", 9.0f, FontStyle.Bold);
    //            }
    //            else if (e.CellElement is GridDataCellElement)
    //            {
    //                e.CellElement.BackColor = Color.FromArgb(89, 114, 150);
    //                e.CellElement.ForeColor = Color.White;
    //                e.CellElement.BorderColor = Color.FromArgb(134, 138, 150);
    //                e.CellElement.Font = new Font("Tahoma", 8.25f);
    //            }
    //        }
    //        else if (e.CellElement.IsSelected)
    //        {
    //            e.CellElement.BorderBoxStyle = BorderBoxStyle.SingleBorder;
    //            if (e.CellElement is GridHeaderCellElement)
    //            {
    //                e.CellElement.DrawFill = true;
    //                e.CellElement.BackColor = Color.FromArgb(74, 101, 140);
    //                e.CellElement.ForeColor = Color.White;
    //                e.CellElement.BorderColor = Color.FromArgb(134, 138, 150);
    //                e.CellElement.Font = new Font("Tahoma", 9.0f, FontStyle.Bold);
    //            }
    //            else if (e.CellElement is GridRowHeaderCellElement)
    //            {
    //            }
    //            else if (e.CellElement is GridDataCellElement)
    //            {
    //                e.CellElement.BackColor = Color.FromArgb(89, 114, 150);
    //                e.CellElement.ForeColor = Color.White;
    //                e.CellElement.BorderColor = Color.FromArgb(134, 138, 150);
    //                e.CellElement.Font = new Font("Tahoma", 8.25f);
    //            }
    //        }
    //        else
    //        {
    //            e.CellElement.BorderBoxStyle = BorderBoxStyle.FourBorders;
    //            if (e.CellElement is GridHeaderCellElement)
    //            {
    //                e.CellElement.DrawFill = true;
    //                e.CellElement.BorderBottomWidth = 0;
    //                e.CellElement.BorderLeftWidth = 0;

    //                e.CellElement.BackColor = Color.FromArgb(213, 216, 226);
    //                e.CellElement.ForeColor = Color.Black;
    //                e.CellElement.BorderColor = Color.FromArgb(134, 138, 150);

    //                e.CellElement.Font = new Font("Tahoma", 9.0f);

    //            }
    //            else if (e.CellElement is GridRowHeaderCellElement)
    //            {
    //                e.CellElement.BorderBottomWidth = 0;
    //                e.CellElement.BorderLeftWidth = 0;

    //                e.CellElement.BackColor = Color.FromArgb(213, 216, 226);
    //                e.CellElement.ForeColor = Color.Black;
    //                e.CellElement.BorderColor = Color.FromArgb(134, 138, 150);

    //                e.CellElement.Font = new Font("Tahoma", 8.25f);
    //            }
    //            else if (e.CellElement is GridDataCellElement)
    //            {
    //                e.CellElement.BorderTopWidth = 0;
    //                e.CellElement.BorderLeftWidth = 0;

    //                e.CellElement.BackColor = Color.FromArgb(255, 255, 255);
    //                e.CellElement.ForeColor = Color.FromArgb(68, 68, 68);
    //                e.CellElement.BorderColor = Color.FromArgb(134, 138, 150);

    //                e.CellElement.Font = new Font("Tahoma", 8.25f);
    //            }
    //        }
    //    }

    //    void KRadGridView_Initialized(object sender, EventArgs e)
    //    {
    //        this.ThemeName = TechSun.Theme.KRadTheme.Windows8;

    //        this.AllowAutoSizeColumns = false;
    //        this.AllowColumnChooser = false;
    //        this.AllowColumnHeaderContextMenu = false;
    //        this.AllowColumnReorder = false;
    //        this.AllowMultiColumnSorting = false;
    //        this.AllowRowResize = false;
    //        this.AutoGenerateColumns = false;
    //        this.AutoSizeColumnsMode = GridViewAutoSizeColumnsMode.Fill;
    //        this.EnableHotTracking = false;
    //        this.ShowCellErrors = false;
    //        this.ShowFilteringRow = false;
    //        this.ShowNoDataText = false;
    //        this.ShowRowErrors = false;

    //        this.MultiSelect = true;
    //        this.EnableGrouping = false;
    //        this.AllowAddNewRow = false;
    //        this.AllowDeleteRow = false;
    //        this.AllowRowReorder = false;
    //        this.GridViewElement.DrawBorder = true;
    //        this.AllowCellContextMenu = true;
    //        this.ShowRowHeaderColumn = false;
    //        this.EnableSorting = false;
    //        this.MasterTemplate.EnableSorting = false;
    //        this.SelectionMode = GridViewSelectionMode.CellSelect;
    //    }

    //}
}
