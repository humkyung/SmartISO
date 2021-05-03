using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace IsoDwg.NET
{
	public partial class FormResult : Form
	{
		public List<QCCheckCommand.Result> ErrorResult { get; set; }

		public FormResult()
		{
			InitializeComponent();
		}

		private void buttonOK_Click(object sender, EventArgs e)
		{
			DialogResult = DialogResult.OK;
		}

		private void buttonClose_Click(object sender, EventArgs e)
		{
			DialogResult = DialogResult.Cancel;
		}

		/// <summary>
		/// form load
		/// </summary>
		/// <author>humkyung</author>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void FormResult_Load(object sender, EventArgs e)
		{
			if (null != ErrorResult)
			{
				List<string> oGroupNameList = new List<string>();
				foreach (QCCheckCommand.Result oErr in ErrorResult)
				{
					ListViewGroup oGroup = null;
					if (!oGroupNameList.Contains(oErr.FileName))
					{
						oGroup = listViewErrList.Groups.Add(oErr.FileName, oErr.FileName);
						oGroupNameList.Add(oErr.FileName);
					}
					else
					{
						oGroup = listViewErrList.Groups[oErr.FileName];
					}

					ListViewItem oItem = listViewErrList.Items.Add(oErr.FileName);
					oItem.SubItems.Add(oErr.Item);
					oItem.SubItems.Add(oErr.ErrString);
					oItem.ImageIndex = ("-1" == oErr.Item) ? 0 : 1;
					oItem.Group = oGroup;
				}
			}
		}
	}
}
