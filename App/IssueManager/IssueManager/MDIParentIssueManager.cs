using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using framework.core;

namespace SmartISO
{
    public partial class MDIParentIssueManager : Form
    {
        private int childFormNumber = 0;

        private string _sProjectNo = string.Empty;
        private string _sProjectPath = string.Empty;
        private IssueManager _frmIssueManager = null;
        private ServerMachine _frmServerMachine = null;
		private FrameworkCore _oFrameworkCore = new FrameworkCore();

		/// <summary>
		/// create and return business logic
		/// </summary>
		private CBusinessLogic _oBusinessLogic = new CBusinessLogic();
		public CBusinessLogic BusinessLogic
		{
			get { return _oBusinessLogic; }
		}

        public MDIParentIssueManager(string sProjectNo, string sProjectPath)
        {
            InitializeComponent();

			AppDocData.frameworkCore = this._oFrameworkCore;

            this._sProjectNo = sProjectNo;
            this._sProjectPath = sProjectPath;

			BusinessLogic.DatabasePath = sProjectPath + @"\Database\" + sProjectNo + ".mdb";
			BusinessLogic.IniFilePath = sProjectPath + @"\Setting\" + sProjectNo + ".ini";
			BusinessLogic.ProjectPath = sProjectPath;	/// 2011.06.13 added by humkyung
			BusinessLogic.IssuePath = sProjectPath + @"\ISSUE"; /// 2012.05.16 added by humkyung
        }

        private void ShowNewForm(object sender, EventArgs e)
        {
            Form childForm = new Form();
            childForm.MdiParent = this;
            childForm.Text = "Window " + childFormNumber++;
            childForm.Show();
        }

        private void OpenFile(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            openFileDialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
            if (openFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                string FileName = openFileDialog.FileName;
            }
        }

        private void SaveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Personal);
            saveFileDialog.Filter = "Text Files (*.txt)|*.txt|All Files (*.*)|*.*";
            if (saveFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                string FileName = saveFileDialog.FileName;
            }
        }

        private void ExitToolsStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void CutToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        private void CopyToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        private void PasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
        }

        private void ToolBarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            toolStrip.Visible = toolBarToolStripMenuItem.Checked;
        }

        private void StatusBarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            statusStrip.Visible = statusBarToolStripMenuItem.Checked;
        }

        private void CascadeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LayoutMdi(MdiLayout.Cascade);
        }

        private void TileVerticalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LayoutMdi(MdiLayout.TileVertical);
        }

        private void TileHorizontalToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LayoutMdi(MdiLayout.TileHorizontal);
        }

        private void ArrangeIconsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            LayoutMdi(MdiLayout.ArrangeIcons);
        }

        private void CloseAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            foreach (Form childForm in MdiChildren)
            {
                childForm.Close();
            }
        }

        /// <summary>
        /// load form
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void MDIParentIssueManager_Load(object sender, EventArgs e)
        {
			this.BusinessLogic.ReadSetting();

            localMachineToolStripButton.Select();/// = true;
            localMachineToolStripButton.Checked = true;

			AppDocData.frameworkCore.InitFramework();
        }

		/// <summary>
		/// 
		/// </summary>
		/// <date>2012.08.20</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
        private void serverMachinetoolStripButton_Click(object sender, EventArgs e)
        {
            localMachineToolStripButton.Checked = false;
			this.UpdateToolStripButton.Enabled = false;

            serverMachineToolStripButton.Select();
            serverMachineToolStripButton.Checked = true;

            this._frmIssueManager.Hide();
            this._frmServerMachine.Show();
        }

		/// <summary>
		/// 
		/// </summary>
		/// <date>2012.08.20</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
        private void localMachineToolStripButton_Click(object sender, EventArgs e)
        {
            serverMachineToolStripButton.Checked = false;

            localMachineToolStripButton.Select();/// = true;
            localMachineToolStripButton.Checked = true;
			this.UpdateToolStripButton.Enabled = true;

            this._frmServerMachine.Hide();
            this._frmIssueManager.Show();
        }

		/// <summary>
		/// update remote database with local database
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.20</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void UpdateToolStripButton_Click(object sender, EventArgs e)
		{
			FormIssueConfirm frm = new FormIssueConfirm();
			if (DialogResult.Yes != frm.ShowDialog(this)) return;

			AppDocData.frameworkCore.Logger.Reset();
			using (framework.core.forms.LoadingIndicator oIndicator = new framework.core.forms.LoadingIndicator(this))
			{
				oIndicator.Start();

				if (true == frm.CheckUpdateDatabase)
				{
					this._frmIssueManager.UpdateRemoteDatabase();
				}
				if(true == frm.CheckTransferIsoDwg)
				{
					this._frmIssueManager.TransferIsoDwgToFtpServer();
				}
			}
			AppDocData.frameworkCore.Logger.ShowErrorResult(this);
		}

		/// <summary>
		/// show setting form
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2012.08.21</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void optionsToolStripMenuItem_Click(object sender, EventArgs e)
		{
			FormRemoteDatabaseSetting frmSetting = new FormRemoteDatabaseSetting(this.BusinessLogic);
			frmSetting.ShowDialog(this);
		}

		/// <summary>
		/// form shown
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void MDIParentIssueManager_Shown(object sender, EventArgs e)
		{
			Application.DoEvents();

			this._frmIssueManager = new IssueManager(this._sProjectNo, this._sProjectPath, this.BusinessLogic);
			this._frmIssueManager.Dock = DockStyle.Fill;
			this.panelMain.Controls.Add(this._frmIssueManager);
			this._frmServerMachine = new ServerMachine(this.BusinessLogic);
			this._frmServerMachine.Dock = DockStyle.Fill;
			this.panelMain.Controls.Add(this._frmServerMachine);
		}
    }
}
