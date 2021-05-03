using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Data.OracleClient;

using framework.core;

namespace SmartISO
{
	public partial class FormRemoteDatabaseSetting : Form
	{
		private CBusinessLogic _oBusinessLogic = null;
		private string _sIniFilePath = string.Empty;
		private string _sRemoteDatabaseAppName = "Remote Database";
		private string _sFtpServerAppName = "Ftp Server";

		public FormRemoteDatabaseSetting(CBusinessLogic oBusinessLogic)
		{
			InitializeComponent();

			_oBusinessLogic = oBusinessLogic;
			_sIniFilePath = _oBusinessLogic.IniFilePath;
		}

		private void buttonClose_Click(object sender, EventArgs e)
		{
			DialogResult = DialogResult.Cancel;
		}

		/// <summary>
		/// save settings
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.21</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void buttonOK_Click(object sender, EventArgs e)
		{
			this._oBusinessLogic.RemoteDatabaseSetting.sHost = this.textBoxHost.Text;
			this._oBusinessLogic.RemoteDatabaseSetting.sService = this.textBoxService.Text;
			this._oBusinessLogic.RemoteDatabaseSetting.sId = this.textBoxId.Text;
			this._oBusinessLogic.RemoteDatabaseSetting.sPassword = this.textBoxPassword.Text;

			AppDocData.FtpServerSetting.sHost = this.textBoxFtpHost.Text;
			AppDocData.FtpServerSetting.sId = this.textBoxFtpId.Text;
			AppDocData.FtpServerSetting.sPassword = this.textBoxFtpPassword.Text;
			AppDocData.FtpServerSetting.sRemoteDirectory = this.textBoxRemoteDirectory.Text;

			this._oBusinessLogic.SaveSetting();

			DialogResult = DialogResult.OK;
		}

		/// <summary>
		/// form load
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.21</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void FormRemoteDatabaseSetting_Load(object sender, EventArgs e)
		{
			this.textBoxHost.Text = this._oBusinessLogic.RemoteDatabaseSetting.sHost;
			this.textBoxService.Text = this._oBusinessLogic.RemoteDatabaseSetting.sService;
			this.textBoxId.Text = this._oBusinessLogic.RemoteDatabaseSetting.sId;
			this.textBoxPassword.Text = this._oBusinessLogic.RemoteDatabaseSetting.sPassword;

			this.textBoxFtpHost.Text = AppDocData.FtpServerSetting.sHost;
			this.textBoxFtpId.Text = AppDocData.FtpServerSetting.sId;
			this.textBoxFtpPassword.Text = AppDocData.FtpServerSetting.sPassword;
			this.textBoxRemoteDirectory.Text = AppDocData.FtpServerSetting.sRemoteDirectory;
		}

		/// <summary>
		/// do connection test
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.21</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void buttonConnectionTest_Click(object sender, EventArgs e)
		{
			try
			{
				using (MyOracleConnection Oracleconn = new MyOracleConnection())
				{
					if (true == Oracleconn.Connect(textBoxHost.Text, textBoxService.Text, textBoxId.Text, textBoxPassword.Text))
					{
						MessageBox.Show("Connection test is success", "MSG", MessageBoxButtons.OK, MessageBoxIcon.Information);
					}
				}
			}
			catch (OracleException ex)
			{
				MessageBox.Show(ex.Message, "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
				this.buttonConnectionTest.ImageIndex = 1;
			}   
		}
	}
}
