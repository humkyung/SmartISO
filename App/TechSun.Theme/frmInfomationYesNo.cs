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
using TechSun.Theme;

namespace TechSun.Theme
{
    public partial class frmInformationYesNo : KRadForm
    {
        //string m_rDetail = string.Empty;
        MessageBoxButtons messageBoxButtons;
        public frmInformationYesNo(string sInformationText, string p_rCaption = "Confirm", MessageBoxButtons p_messageBoxButtons = MessageBoxButtons.YesNo)
        {
            InitializeComponent();

            messageBoxButtons = p_messageBoxButtons;

            if (!string.IsNullOrWhiteSpace(sInformationText))
            {
                this.TextBoxInfomation.Text = sInformationText;
            }

            if (!string.IsNullOrWhiteSpace(p_rCaption))
            {
                this.Text = p_rCaption;
            }

            this.kRadButtonYes.Select();
            this.BackColor = System.Drawing.Color.White;
            if (messageBoxButtons == MessageBoxButtons.YesNo)
            {
                radButtonNo.Text = "No";
                kRadButtonYes.Text = "Yes";
            }
            else if (messageBoxButtons == MessageBoxButtons.OKCancel)
            {
                //radButtonNo.Text = "Cancel";
                //kRadButtonYes.Text = "OK";
            }
        }

        private void radButtonNo_Click(object sender, EventArgs e)
        {
            if (messageBoxButtons == MessageBoxButtons.YesNo)
            {
                this.DialogResult = DialogResult.No;
            }
            else if (messageBoxButtons == MessageBoxButtons.OKCancel)
            {
                this.DialogResult = DialogResult.Cancel;
            }
        }

        private void radLabel2_Click(object sender, EventArgs e)
        {
            //string command = "mailto:chanwoong.ahn@daelim.co.kr";
            //System.Diagnostics.Process.Start(command); 
        }

        private void kRadButtonYes_Click(object sender, EventArgs e)
        {
            if (messageBoxButtons == MessageBoxButtons.YesNo)
            {
                this.DialogResult = DialogResult.Yes;
            }
            else if (messageBoxButtons == MessageBoxButtons.OKCancel)
            {
                this.DialogResult = DialogResult.OK;
            }
        }
    }
}
