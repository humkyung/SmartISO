using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SmartISO
{
	public partial class FormIssueConfirm : Form
	{
		private bool _bCheckUpdateDatabase = true;
		public bool CheckUpdateDatabase
		{
			get { return _bCheckUpdateDatabase; }
		}

		private bool _bCheckTransferIwoDwg = false;
		public bool CheckTransferIsoDwg
		{
			get { return _bCheckTransferIwoDwg; }
		}

		public FormIssueConfirm()
		{
			InitializeComponent();
		}

		private void checkBoxUpdateDatabase_CheckedChanged(object sender, EventArgs e)
		{
			this._bCheckUpdateDatabase = this.checkBoxUpdateDatabase.Checked;
		}

		private void checkBoxTransferIsoDwg_CheckedChanged(object sender, EventArgs e)
		{
			this._bCheckTransferIwoDwg = this.checkBoxTransferIsoDwg.Checked;
		}

		private void buttonYes_Click(object sender, EventArgs e)
		{
			DialogResult = DialogResult.Yes;
		}

		private void buttonNO_Click(object sender, EventArgs e)
		{
			DialogResult = DialogResult.No;
		}
	}
}
