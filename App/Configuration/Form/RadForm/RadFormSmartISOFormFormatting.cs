using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Telerik.WinControls;

namespace Configuration
{
	public partial class RadFormSmartISOFormFormatting : TechSun.Theme.KRadForm
    {
		private static string _delimiter = ":";
		public static string Delimiter
		{
			get { return _delimiter; }
			set { _delimiter = value; }
		}
		private string _sResult = string.Empty;
		public string Result
		{
			get { return _sResult; }
		}

		private List<string> _InputList = new List<string>();
		public RadFormSmartISOFormFormatting(string result, List<string> InputList)
        {
            InitializeComponent();
			this._sResult = result;
			this._InputList = InputList;
        }


		private void radButtonAdd_Click(object sender, EventArgs e)
		{
			if ((null != this.radListControlData.SelectedItem) && (string.Empty != this.radTextBoxDelimiter.Text))
			{
				Telerik.WinControls.UI.RadListDataItem aItem = new Telerik.WinControls.UI.RadListDataItem(this.radTextBoxDelimiter.Text);
				this.radListControlData.Items.Insert(this.radListControlData.SelectedItem.RowIndex + 1, aItem);

				this.radListControlData.SelectedIndex = this.radListControlData.SelectedItem.RowIndex + 1;
			}
			else
			{
				Telerik.WinControls.UI.RadListDataItem aItem = new Telerik.WinControls.UI.RadListDataItem(this.radTextBoxDelimiter.Text);
				this.radListControlData.Items.Add(aItem);

				this.radListControlData.SelectedIndex = this.radListControlData.Items.Count - 1;
			}
		}

		private void radButtonDelete_Click(object sender, EventArgs e)
		{
			if (null != this.radListControlData.SelectedItem)
			{
				this.radListControlData.Items.Remove(this.radListControlData.SelectedItem);
				if (true == this.radDropDownList.Visible)
				{
					this.radDropDownList.Hide();
				}
			}
		}

		private void kRadButtonUp_Click(object sender, EventArgs e)
		{
			if ((null != this.radListControlData.SelectedItem) && (this.radListControlData.SelectedItem.RowIndex > 0))
			{
				int iSelectedIndex = this.radListControlData.SelectedItem.RowIndex;

				Telerik.WinControls.UI.RadListDataItem aItem = new Telerik.WinControls.UI.RadListDataItem(this.radListControlData.SelectedItem.Text);
				this.radListControlData.Items.Insert(this.radListControlData.SelectedItem.RowIndex - 1, aItem);
				this.radListControlData.Items.RemoveAt(this.radListControlData.SelectedItem.RowIndex);

				this.radListControlData.SelectedIndex = iSelectedIndex - 1;
			}
		}

		private void kRadButtonDown_Click(object sender, EventArgs e)
		{
			if ((null != this.radListControlData.SelectedItem) && (this.radListControlData.SelectedItem.RowIndex < this.radListControlData.Items.Count - 1))
			{
				int iSelectedIndex = this.radListControlData.SelectedItem.RowIndex;
				Telerik.WinControls.UI.RadListDataItem aItem = new Telerik.WinControls.UI.RadListDataItem(this.radListControlData.SelectedItem.Text);
				this.radListControlData.Items.RemoveAt(this.radListControlData.SelectedItem.RowIndex);
				this.radListControlData.Items.Insert(iSelectedIndex + 1, aItem);

				this.radListControlData.SelectedIndex = this.radListControlData.SelectedItem.RowIndex + 1;
			}
		}

		private void RadFormSmartISOFormFormatting_Load(object sender, EventArgs e)
		{
			radDropDownList.Parent = this.radListControlData;

			/// fill combobox with line data columns - 2012.07.12 added by humkyung
			/// //changed string to APPoracleInfo
			foreach (string str in this._InputList)
			{
				if (false == this.radDropDownList.Items.Contains(str))
				{
					this.radDropDownList.Items.Add(str);
				}
			}

			/// display inputted string on list box - 2012.07.17 added by humkyung
			if (string.Empty != this._sResult)
			{
				string[] tokens = this._sResult.Split(RadFormSmartISOFormFormatting.Delimiter.ToCharArray());
				foreach (string aValue in tokens)
				{
					this.radListControlData.Items.Add(aValue);
				}
			}
		}

		private void radButtonClose_Click(object sender, EventArgs e)
		{
			this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
		}

		private void radButtonOK_Click(object sender, EventArgs e)
		{
			_sResult = string.Empty;
			foreach (Telerik.WinControls.UI.RadListDataItem aItem in this.radListControlData.Items)
			{
				_sResult += aItem.Text + RadFormSmartISOFormFormatting.Delimiter;
			}
			_sResult = _sResult.TrimEnd(RadFormSmartISOFormFormatting.Delimiter.ToCharArray());

			this.DialogResult = System.Windows.Forms.DialogResult.OK;
		}

		private void radListControlData_DoubleClick(object sender, EventArgs e)
		{
			if (null != this.radListControlData.SelectedItem)
			{
				radDropDownList.Left = 0;
				radDropDownList.Top = (this.radListControlData.SelectedItem.RowIndex) * this.radListControlData.ItemHeight;
				radDropDownList.Width = this.radListControlData.Width;
				radDropDownList.Show();

				if (radDropDownList.Items.Contains(this.radListControlData.SelectedItem.Text))
				{
					radDropDownList.Text = this.radListControlData.SelectedItem.Text;
				}
			}
		}

		private void radListControlData_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
		{
			if (true == radDropDownList.Visible)
			{
				radDropDownList.Hide();
			}
		}

		private void radListControlData_Click(object sender, EventArgs e)
		{
			if (true == radDropDownList.Visible)
			{
				radDropDownList.Hide();
			}
		}

		private void radDropDownList_SelectedValueChanged(object sender, EventArgs e)
		{
			if (null != this.radListControlData.SelectedItem)
			{
				this.radListControlData.SelectedItem.Text = this.radDropDownList.Text;
			}
		}

		private void radButtonNew_Click(object sender, EventArgs e)
		{
			this.radListControlData.Items.Add("");

			this.radListControlData.SelectedIndex = this.radListControlData.Items.Count - 1;
			radDropDownList.Left = 0;
			radDropDownList.Top = (this.radListControlData.Items.Count - 1) * this.radListControlData.ItemHeight;
			radDropDownList.Width = this.radListControlData.Width;
			radDropDownList.Show();
		}

		private void radDropDownList_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
		{
			if (null != this.radListControlData.SelectedItem)
			{
				this.radListControlData.SelectedItem.Text = this.radDropDownList.Text;
			}
		}
    }
}
