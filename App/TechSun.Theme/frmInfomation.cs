using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Reflection;

using Telerik.WinControls;
using Telerik.WinControls.Enumerations;
using Telerik.WinControls.UI;
using Telerik.WinControls.RichTextBox.Model.Styles;
using Telerik.WinControls.RichTextBox.Model;
using TechSun.Theme;

namespace TechSun.Theme
{
    public enum TitleCation
    {
        [Description("Warning")]
        Warning = 1,
        [Description("Information")]
        Information = 2,
        [Description("Save")]
        Save = 3,
        [Description("Message")]
        Message = 4,
        [Description("Add")]
        Add = 5,
        [Description("Delete")]
        Delete = 6,
        [Description("Modify")]
        Modify = 7,
        [Description("Drawing")]
        Drawing = 8,
        [Description("Item Type")]
        ItemType = 9,
        [Description("Update Autdit")]
        UpdateAutdit = 10,
        [Description("Comment Column")]
        CommentColumn = 11,
        [Description("Audit Column")]
        AuditColumn = 12,
        [Description("Update")]
        Update = 13,
        [Description("Attribute")]
        Attribute = 14,
    }


    public partial class frmInfomation : KRadForm
    {
        public delegate void EventDelegate();
        public event EventDelegate ClickExportTagList;

        public frmInfomation()
        {

        }
        public frmInfomation(string sInformationText)
        {
            InitializeComponent();

            if (!string.IsNullOrWhiteSpace(sInformationText))
            {
                this.TextBoxInfomation.Text = sInformationText;
            }

            this.Text = "Information";
            this.BackColor = System.Drawing.Color.White;
            this.radButtonExportTagList.Visible = false;
        }
        public frmInfomation(string sInformationText, bool p_flagVisibleExportTagListButton)
        {
            InitializeComponent();

            if (!string.IsNullOrWhiteSpace(sInformationText))
            {
                this.TextBoxInfomation.Text = sInformationText;
            }

            this.Text = "Information";
            this.BackColor = System.Drawing.Color.White;
            if (p_flagVisibleExportTagListButton == false)
            {
                this.radButtonExportTagList.Visible = false;
            }
            else
            {
                this.radButtonExportTagList.Click += (e1, e2) =>
                    {
                        this.ClickExportTagList();
                    };
            }
        }
        public frmInfomation(string sInformationText, TitleCation titleCaption = TitleCation.Information)
        {
            InitializeComponent();

            if (!string.IsNullOrWhiteSpace(sInformationText))
            {
                this.TextBoxInfomation.Text = sInformationText;
            }

            this.Text = MessageInfo.GetDescription(titleCaption);
            this.Text = Text.PadLeft(3 + Text.Length);
            this.BackColor = System.Drawing.Color.White;

            this.radButtonExportTagList.Visible = false;
        }


        public void radButtonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void radLabel2_Click(object sender, EventArgs e)
        {
            //string command = "mailto:chanwoong.ahn@daelim.co.kr";
            //System.Diagnostics.Process.Start(command); 
        }
    }

    public static class MessageInfo
    {
        public static string GetDescription(this Enum value)
        {
            if (value == null)
            {
                throw new ArgumentNullException("value");
            }

            string description = value.ToString();
            FieldInfo fieldInfo = value.GetType().GetField(description);
            DescriptionAttribute[] attributes =
               (DescriptionAttribute[])
             fieldInfo.GetCustomAttributes(typeof(DescriptionAttribute), false);

            if (attributes != null && attributes.Length > 0)
            {
                description = attributes[0].Description;
            }
            return description;
        }
    }
}
