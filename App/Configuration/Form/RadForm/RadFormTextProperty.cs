using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Telerik.WinControls;
using TechSun.Theme;

namespace Configuration
{
    public partial class RadFormTextProperty : KRadForm
    {
		private string _TextColor = string.Empty;
		public string TextColor
		{
			get { return _TextColor; }
			set { _TextColor = value; }
		}
		private string _TextStyle = string.Empty;
		public string TextStyle
		{
			get { return _TextStyle; }
			set { _TextStyle = value; }
		}
		private string _TextHeight = string.Empty;
		public string TextHeight
		{
			get { return _TextHeight; }
			set { _TextHeight = value; }
		}

		private bool IsFormLoaded = false;

		public RadFormTextProperty()
        {
            InitializeComponent();
        }

		private void radButtonOK_Click(object sender, EventArgs e)
		{
			DialogResult = DialogResult.OK;
		}

		private void radButtonClose_Click(object sender, EventArgs e)
		{
			DialogResult = DialogResult.Cancel;
		}

		private void RadFormTextProperty_Load(object sender, EventArgs e)
		{
			radDropDownListTextColor.Text = TextColor;
			radLabelSelectedColor.BackColor = RadFormColorTable.GetRGBFromIndex(TextColor);
			radDropDownListTextStyle.Text = TextStyle;
			radTextBoxHeight.Text = TextHeight;

			IsFormLoaded = true;
		}

		private void radTextBoxHeight_TextChanged(object sender, EventArgs e)
		{
			TextHeight = radTextBoxHeight.Text;
		}

		private void radButtonTextColor_Click(object sender, EventArgs e)
		{
			RadFormColorTable frmColorTable = new RadFormColorTable();
			frmColorTable.ColorIndex = radDropDownListTextColor.Text;
			if (DialogResult.OK == frmColorTable.ShowDialog(this))
			{
				TextColor = radDropDownListTextColor.Text = frmColorTable.ColorIndex;
				radLabelSelectedColor.BackColor = RadFormColorTable.GetRGBFromIndex(frmColorTable.ColorIndex);
			}
		}

		private void radButtonTextStyle_Click(object sender, EventArgs e)
		{
			FrmTextStyle frm = new FrmTextStyle();
			frm.ShowDialog(this);
		}

		private void radDropDownListTextStyle_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
		{
			TextStyle = radDropDownListTextStyle.Text;
		}

		private void radDropDownListTextColor_TextChanged(object sender, EventArgs e)
		{
			if (this.IsFormLoaded)
			{
				TextColor = radDropDownListTextColor.Text;
				radLabelSelectedColor.BackColor = RadFormColorTable.GetRGBFromIndex(radDropDownListTextColor.Text);
			}
		}
    }
}
