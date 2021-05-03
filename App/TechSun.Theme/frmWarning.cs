using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Telerik.WinControls;
using Telerik.WinControls.Enumerations;
using Telerik.WinControls.UI;
using Telerik.WinControls.RichTextBox.Model.Styles;
using Telerik.WinControls.RichTextBox.Model;


namespace TechSun.Theme
{
    public partial class frmWarning : KRadForm
    {
        //string m_rDetail = string.Empty;
        public frmWarning(string p_rDetail)
        {
            InitializeComponent();

            StyleDefinition style = new StyleDefinition();
            style.SetPropertyValue(Span.FontFamilyProperty, "맑은 고딕");
            style.SetPropertyValue(Span.FontSizeProperty, Unit.PointToDip(8.25f));
            style.SetPropertyValue(Span.ForeColorProperty, Color.Black);

            this.radRichTextBoxWarning.Document.Insert(p_rDetail, style);

            this.Text = "Warning";
            this.BackColor = System.Drawing.Color.White;

            this.FormElement.TitleBar.FillPrimitive.BackColor = System.Drawing.Color.FromArgb(0, 39, 94);
            this.FormElement.TitleBar.FillPrimitive.GradientStyle = Telerik.WinControls.GradientStyles.Solid;
            this.FormElement.TitleBar.TitlePrimitive.ForeColor = System.Drawing.Color.FromArgb(255, 255, 255);

            this.FormElement.TitleBar.BorderPrimitive.ForeColor = System.Drawing.Color.FromArgb(0, 39, 94);

            this.FormElement.ImageBorder.BackColor = System.Drawing.Color.FromArgb(0, 39, 94);
            this.FormElement.Border.ForeColor = System.Drawing.Color.FromArgb(0, 39, 94);

            this.TopMost = true;

        }

        private void radToggleButtonDetail_ToggleStateChanging(object sender, StateChangingEventArgs args)
        {
            if ((this.radToggleButtonDetail.ToggleState == ToggleState.On))
            {
                this.radToggleButtonDetail.Image = global::TechSun.Theme.Properties.Resources.arrow_cdown;
                this.Size = new Size(this.Width, 326);
                this.radRichTextBoxWarning.Visible = false;
                this.AutoSize = false;

            }
            else
            {
                this.radToggleButtonDetail.Image = global::TechSun.Theme.Properties.Resources.arrow_cup;
                this.Size = new Size(this.Width, 561);
                this.radRichTextBoxWarning.Visible = true;
                this.AutoSize = true;
            }
        }

        private void radButtonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
        }

        private void radLabel2_Click(object sender, EventArgs e)
        {
            //string command = "mailto:chanwoong.ahn@daelim.co.kr";
            //System.Diagnostics.Process.Start(command); 
        }

        private void radToggleButtonDetail_MouseEnter(object sender, EventArgs e)
        {
            if ((this.radToggleButtonDetail.ToggleState == ToggleState.On))
            {
                this.radToggleButtonDetail.Image = global::TechSun.Theme.Properties.Resources.arrow_cup_on;
            }
            else
            {
                this.radToggleButtonDetail.Image = global::TechSun.Theme.Properties.Resources.arrow_cdown_on;
            }

            this.radToggleButtonDetail.ButtonElement.ButtonFillElement.BackColor = Color.FromArgb(0, 39, 94);
            this.radToggleButtonDetail.ButtonElement.ForeColor = Color.FromArgb(255, 255, 255);
            this.radToggleButtonDetail.ButtonElement.BorderElement.ForeColor = Color.FromArgb(0, 39, 94);
        }

        private void radToggleButtonDetail_MouseLeave(object sender, EventArgs e)
        {
            if ((this.radToggleButtonDetail.ToggleState == ToggleState.On))
            {
                this.radToggleButtonDetail.Image = global::TechSun.Theme.Properties.Resources.arrow_cup;
            }
            else
            {
                this.radToggleButtonDetail.Image = global::TechSun.Theme.Properties.Resources.arrow_cdown;
            }

            this.radToggleButtonDetail.ButtonElement.ButtonFillElement.BackColor = Color.White;
            this.radToggleButtonDetail.ButtonElement.ForeColor = Color.FromArgb(90, 90, 90);
            this.radToggleButtonDetail.ButtonElement.BorderElement.ForeColor = Color.FromArgb(153, 153, 153);
        }
    }
}
