using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace SmartISO.NET
{
	public partial class ProgressLogDlg : Form
	{
		private static ProgressLogDlg Instance_;
		public static ProgressLogDlg Instance
		{
			get
			{
				if (ProgressLogDlg.Instance_ == null)
					ProgressLogDlg.Instance_ = new ProgressLogDlg();
				return ProgressLogDlg.Instance_;
			}
		}
		public event EventHandler StartWork;

		public ProgressLogDlg()
		{
			InitializeComponent();
			this.Disposed += new EventHandler(this.ProgressLogDlg_Disposed);
		}

		private void ProgressLogDlg_Disposed(object sender, EventArgs e)
		{
			ProgressLogDlg.Instance_ = (ProgressLogDlg)null;
		}

		private void ProgressLogDlg_Load(object sender, EventArgs e)
		{
			CenterToParent();
			if (null != StartWork) StartWork(ProgressLogDlg.Instance, e);
		}

		private void buttonClose_Click(object sender, EventArgs e)
		{
			Close();
		}

		/// <summary>
		/// save data
		/// </summary>
		/// <author>humkyung</author>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void buttonSaveData_Click(object sender, EventArgs e)
		{
			SaveFileDialog dlg = new SaveFileDialog() {DefaultExt="txt",Filter="Text File(*.txt)|*txt"};
			if (DialogResult.OK == dlg.ShowDialog())
			{
				using (System.IO.StreamWriter sw = new System.IO.StreamWriter(dlg.FileName))
				{
					foreach(ListViewGroup oGroup in listViewLog.Groups)
					{
						string sValue = string.Empty;
						foreach (ListViewItem oItem in oGroup.Items)
						{
							if (sValue.Length > 0) sValue += ",";
							sValue += oItem.SubItems[1].Text;
						}
						sw.WriteLine(string.Format("{0},{1}", oGroup.Name , sValue));
					}
				}
			}
		}
	}
}
