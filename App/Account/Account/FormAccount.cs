using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace Account
{
	public partial class FormAccount : Form
	{
		public FormAccount()
		{
			InitializeComponent();
			this.Text = string.Format("{0} ver {1}", Application.ProductName, Application.ProductVersion);
		}

		private void FormAccount_Load(object sender, EventArgs e)
		{
			//dataGridViewAccount.AutoGenerateColumns = false;
			//DataGridViewComboBoxColumn cboColumn = dataGridViewAccount.Columns["Type"] as DataGridViewComboBoxColumn;
			//cboColumn.DataSource = new string[] { "Admin", "User" };

			ControlHelper.SuspendDrawing(dataGridViewAccount);
			AppDocData.AppDataPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "SmartISO");
			try
			{
				using (AppSQLiteDatabase database = new AppSQLiteDatabase() { FilePath = Path.Combine(AppDocData.AppDataPath, "Account.db") })
				{
					using (System.Data.Common.DbTransaction txn = database.BeginTransaction())
					{
						string sql = string.Empty;
						using (StreamReader oStreamReader = new StreamReader(Path.Combine(Path.GetDirectoryName(Application.ExecutablePath) , @"..\Backup\Account.sql")))
						{
							sql = oStreamReader.ReadToEnd();
						}
						database.ExecuteNonQuery(sql);

						using (DataSet ds = database.ExecuteDataSet(database.GetSqlStringCommand("SELECT * FROM Account")))
						{
							dataGridViewAccount.DataSource = ds.Tables[0].DefaultView;
							dataGridViewAccount.Columns.Remove("UserType");
							dataGridViewAccount.Columns.Insert(2, new DataGridViewComboBoxColumn() { Name = "UserType", DataPropertyName = "UserType", DataSource = new string[] { "Admin", "User" } });
						}

						dataGridViewAccount.AutoResizeColumns();
						txn.Commit();
					}
				}
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			ControlHelper.ResumeDrawing(dataGridViewAccount);
		}

		/// <summary>
		/// save data
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2015.08.19</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void buttonOK_Click(object sender, EventArgs e)
		{
			AppDocData.AppDataPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "SmartISO");
			using (AppSQLiteDatabase database = new AppSQLiteDatabase() { FilePath = Path.Combine(AppDocData.AppDataPath, "Account.db") })
			{
				try
				{
					#region write account
					using (System.Data.Common.DbTransaction tr = database.BeginTransaction())
					{
						database.ExecuteNonQuery("DELETE FROM Account");
						string sColumns = string.Empty;
						foreach (DataGridViewColumn aCol in dataGridViewAccount.Columns)
						{
							if (!string.IsNullOrEmpty(sColumns)) sColumns += ",";
							sColumns += aCol.Name;
						}
						foreach (DataGridViewRow aRow in dataGridViewAccount.Rows)
						{
							if (dataGridViewAccount.Rows.Count - 1 == aRow.Index) break;
							string sql = string.Format(@"INSERT INTO Account({0}) VALUES(", sColumns);
							foreach (DataGridViewColumn aCol in dataGridViewAccount.Columns)
							{
								object oValue = aRow.Cells[aCol.Name].Value;
								sql += (null != oValue) ? string.Format("'{0}',", oValue.ToString().Replace("'", "''")) : "'',";
							}
							sql = sql.TrimEnd(",".ToCharArray());
							sql += ")";

							database.ExecuteNonQuery(sql);
						}
						tr.Commit();
						MessageBox.Show("Save is done", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
					}
					#endregion
				}
				catch (Exception ex)
				{
					MessageBox.Show(ex.Message);
				}
			}
		}

		/// <summary>
		/// exit application
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2015.08.19</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void buttonClose_Click(object sender, EventArgs e)
		{
			Application.Exit();
		}

		/// <summary>
		/// display current account info.
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2015.09.21</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void dataGridViewAccount_CurrentCellChanged(object sender, EventArgs e)
		{
			if (null == dataGridViewAccount.CurrentCell) return;

			int iRowIndex = dataGridViewAccount.CurrentCell.RowIndex;
			if ((iRowIndex >= 0) && (iRowIndex < dataGridViewAccount.Rows.Count))
			{
				foreach (DataGridViewColumn oCol in dataGridViewAccount.Columns)
				{
					if ("PrjName" == oCol.Name)
					{
						textBoxPrjName.Text = dataGridViewAccount.Rows[iRowIndex].Cells[oCol.Name].Value.ToString();
					}
					else if ("PrjDesc" == oCol.Name)
					{
						textBoxPrjDesc.Text = dataGridViewAccount.Rows[iRowIndex].Cells[oCol.Name].Value.ToString();
					}
					else if ("UserType" == oCol.Name)
					{
						comboBoxUserType.Text = dataGridViewAccount.Rows[iRowIndex].Cells[oCol.Name].Value.ToString();
					}
					else if ("UserName" == oCol.Name)
					{
						textBoxUserName.Text = dataGridViewAccount.Rows[iRowIndex].Cells[oCol.Name].Value.ToString();
					}
					else if ("UserPassword" == oCol.Name)
					{
						textBoxUserPassword.Text = dataGridViewAccount.Rows[iRowIndex].Cells[oCol.Name].Value.ToString();
					}
					else if ("PrjCode" == oCol.Name)
					{
						textBoxPrjCode.Text = dataGridViewAccount.Rows[iRowIndex].Cells[oCol.Name].Value.ToString();
					}
					else if ("ClientName" == oCol.Name)
					{
						textBoxClientName.Text = dataGridViewAccount.Rows[iRowIndex].Cells[oCol.Name].Value.ToString();
					}
					else if ("Folder" == oCol.Name)
					{
						textBoxFolder.Text = dataGridViewAccount.Rows[iRowIndex].Cells[oCol.Name].Value.ToString();
					}
					else if ("Host".ToUpper() == oCol.Name.ToUpper())
					{
						textBoxDatabase_Host.Text = dataGridViewAccount.Rows[iRowIndex].Cells[oCol.Name].Value.ToString();
					}
					else if ("ID" == oCol.Name.ToUpper())
					{
						textBoxDatabase_ID.Text = dataGridViewAccount.Rows[iRowIndex].Cells[oCol.Name].Value.ToString();
					}
					else if ("Password".ToUpper() == oCol.Name.ToUpper())
					{
						textBoxDatabase_Password.Text = dataGridViewAccount.Rows[iRowIndex].Cells[oCol.Name].Value.ToString();
					}
				}
			}
		}

        /// <summary>
        /// 데이타베이스 연결 테스트를 진행한다
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void buttonConnect_Click(object sender, EventArgs e)
        {
            using (AppPostgreSQLDatabase database = new AppPostgreSQLDatabase() { Server = this.textBoxDatabase_Host.Text,
                Id = this.textBoxDatabase_ID.Text, Password = this.textBoxDatabase_Password.Text,
                Database =this.textBoxPrjName.Text})
            {
                try
                {
                    database.Connection.Open();
                    MessageBox.Show("Connection Test is OK", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }
	}
}
