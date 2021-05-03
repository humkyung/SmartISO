using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using ScintillaNET;
using ScriptingInterface;
using Telerik.WinControls.UI;

namespace Configuration.Form.RadForm
{
    public partial class RadFormIssueDateDisplayFormat : TechSun.Theme.KRadForm
    {
		public string DisplayName { get; set; }
		public string Value { get; set; }

		public IEnumerable<int> CheckedIssueDateDisplayFormatList
		{
			get
			{
				var rows = (from GridViewRowInfo row in radGridViewDisplayName.Rows
							where ((null != row.Cells["Check"].Value) && (true.ToString() == row.Cells["Check"].Value.ToString()))
							select row.Index);
				return rows;
			}
		}

		private List<UserControlRevSetting.IssueDateDisplayFormat> IssueDateDisplayFormatList;

        public RadFormIssueDateDisplayFormat(string sSelectedName, List<UserControlRevSetting.IssueDateDisplayFormat> oIssueDateDisplayFormatList)
		{
			InitializeComponent();
			{
				this.scintillaEdit = new Scintilla();
				// 
				// scintillaEdit
				// 
				this.scintillaEdit.Anchor = ((AnchorStyles)((((AnchorStyles.Top | AnchorStyles.Bottom)
				| AnchorStyles.Left)
				| AnchorStyles.Right)));
				this.scintillaEdit.Location = new Point(16, 45);
				this.scintillaEdit.Name = "scintillaEdit";
				this.scintillaEdit.Size = new Size(605, 238);
				this.scintillaEdit.Dock = DockStyle.Fill;
				this.scintillaEdit.Styles.BraceBad.Size = 9F;
				this.scintillaEdit.Styles.BraceLight.Size = 9F;
				this.scintillaEdit.Styles.ControlChar.Size = 9F;
				this.scintillaEdit.Styles.Default.BackColor = SystemColors.Window;
				this.scintillaEdit.Styles.Default.Size = 9F;
				this.scintillaEdit.Styles.IndentGuide.Size = 9F;
				this.scintillaEdit.Styles.LastPredefined.Size = 9F;
				this.scintillaEdit.Styles.LineNumber.Size = 9F;
				this.scintillaEdit.Styles.Max.Size = 9F;
				this.scintillaEdit.TabIndex = 10;

				this.radGroupBox1.Controls.Add(this.scintillaEdit);
			}

            

			this.IssueDateDisplayFormatList = oIssueDateDisplayFormatList;
			this.IssueDateDisplayFormatList.RemoveAt(this.IssueDateDisplayFormatList.FindIndex(param => param.Name == UserControlRevSetting.NewOrEdit));
			this.radGridViewDisplayName.RowCount = this.IssueDateDisplayFormatList.Count;
			foreach (GridViewRowInfo oGridRow in this.radGridViewDisplayName.Rows)
			{
				if (UserControlRevSetting.NewOrEdit == this.IssueDateDisplayFormatList[oGridRow.Index].Name) continue;
				this.radGridViewDisplayName.Rows[oGridRow.Index].Cells["Name"].Value = this.IssueDateDisplayFormatList[oGridRow.Index].Name;
				this.radGridViewDisplayName.Rows[oGridRow.Index].Cells["Name"].ReadOnly = true;
				this.radGridViewDisplayName.Rows[oGridRow.Index].Tag = this.IssueDateDisplayFormatList[oGridRow.Index];
			}
			this.radGridViewDisplayName.BestFitColumns();
		}

		private void RadFormIssueDateDisplayFormat_Load(object sender, EventArgs e)
		{
			this.scintillaEdit.ConfigurationManager.Language = "cs";
			this.scintillaEdit.ConfigurationManager.Configure();
		}

		private void radButtonClose_Click(object sender, EventArgs e)
		{
			if ((null != this.radGridViewDisplayName.CurrentCell) && (null != this.radGridViewDisplayName.CurrentCell.Value))
			{
				this.DisplayName = this.radGridViewDisplayName.CurrentCell.Value.ToString();
				this.Value = this.scintillaEdit.Text;
			}

			this.IssueDateDisplayFormatList.Add(new UserControlRevSetting.IssueDateDisplayFormat()
			{
				Name = UserControlRevSetting.NewOrEdit,
				Content = string.Empty
			});

			this.DialogResult = DialogResult.Cancel;
		}

		private void radButtonOK_Click(object sender, EventArgs e)
		{
			if ((null != this.radGridViewDisplayName.CurrentCell) && (null != this.radGridViewDisplayName.CurrentCell.Value))
			{
				this.DisplayName = this.radGridViewDisplayName.CurrentCell.Value.ToString();
				this.Value = this.scintillaEdit.Text;

				UserControlRevSetting.IssueDateDisplayFormat res = this.IssueDateDisplayFormatList.Find(param => param.Name == this.DisplayName);
				if (null != res)
				{
					res.Content = this.Value;
				}
			}

			this.IssueDateDisplayFormatList.Add(new UserControlRevSetting.IssueDateDisplayFormat()
			{
				Name = UserControlRevSetting.NewOrEdit,
				Content = string.Empty
			});

			this.DialogResult = DialogResult.OK;
		}

		/// <summary>
		/// show content of selected item
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.30</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radGridViewDisplayName_SelectionChanged(object sender, EventArgs e)
		{
			if ((null != this.radGridViewDisplayName.CurrentCell) && ((null != this.radGridViewDisplayName.CurrentCell.Value)) && (this.radGridViewDisplayName.CurrentRow.Index >= 0))
			{
				/// save display format content
				{
					UserControlRevSetting.IssueDateDisplayFormat res = this.IssueDateDisplayFormatList.Find(param => param.Name == this.DisplayName);
					if (null != res)
					{
						res.Content = this.scintillaEdit.Text;
					}
				}
				/// up to here

				{
					UserControlRevSetting.IssueDateDisplayFormat res = this.IssueDateDisplayFormatList.Find(param => param.Name == this.radGridViewDisplayName.CurrentCell.Value.ToString());
					if (null != res)
					{
						this.scintillaEdit.Text = res.Content;
					}
					else
					{
						this.scintillaEdit.Text = string.Empty;
					}
				}

				this.DisplayName = this.radGridViewDisplayName.CurrentCell.Value.ToString();
				this.Value = this.scintillaEdit.Text;
			}
			else if(null != this.scintillaEdit)
			{
			}
		}

		private void radGridViewDisplayName_UserAddingRow(object sender, GridViewRowCancelEventArgs e)
		{
			UserControlRevSetting.IssueDateDisplayFormat res = this.IssueDateDisplayFormatList.Find(param => param.Name == e.Rows[0].Cells[0].Value.ToString());
			if (null != res)
			{
				e.Cancel = true;
			}
		}

		/// <summary>
		/// add a new display format
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.30</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radGridViewDisplayName_UserAddedRow(object sender, GridViewRowEventArgs e)
		{
			try
			{
				if (!string.IsNullOrEmpty(e.Rows[0].Cells[0].Value.ToString()))
				{
					this.IssueDateDisplayFormatList.Add(new UserControlRevSetting.IssueDateDisplayFormat()
					{
						Name = e.Rows[0].Cells[0].Value.ToString(),
						Content = string.Empty
					});

					this.DisplayName = e.Rows[0].Cells[0].Value.ToString();
					this.Value = string.Empty;
				}
			}
			finally
			{
				e.Row.Cells[0].ReadOnly = true;
				this.scintillaEdit.Text = string.Empty;
			}
		}

		private void radButtonEvaluate_Click(object sender, EventArgs e)
		{
			string sScriptCode =
@"using System;
using System.Collections.Generic;
public class Evaluator
{
	public static string ToString(int Year,int Month,int Day)
	{"
	+ this.scintillaEdit.Text +
	@"}
}";
			try
			{
				Cursor.Current = Cursors.WaitCursor;

				ScriptEngine oScriptEngine = new ScriptEngine(sScriptCode);
				this.radTextBoxResult.Text = oScriptEngine.ToString(DateTime.Now.Year, DateTime.Now.Month, DateTime.Now.Day);
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
			finally
			{
				Cursor.Current = Cursors.Default;
			}
		}

		/// <summary>
		/// add a new issue date display format
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.30</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonAdd_Click(object sender, EventArgs e)
		{
			GridViewRowInfo oGridRow = radGridViewDisplayName.Rows.AddNew();
			try
			{
				UserControlRevSetting.IssueDateDisplayFormat oDispFormat = new UserControlRevSetting.IssueDateDisplayFormat()
				{
					Name = string.Empty,
					Content = string.Empty
				};
				this.IssueDateDisplayFormatList.Add(oDispFormat);
				this.radGridViewDisplayName.Rows[oGridRow.Index].Tag = oDispFormat;

				this.DisplayName = string.Empty;
				this.Value = string.Empty;
			}
			finally
			{
				this.scintillaEdit.Text = string.Empty;
			}
		}

		/// <summary>
		/// delete checked issue date display format
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.30</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radButtonDelete_Click(object sender, EventArgs e)
		{
			if (this.CheckedIssueDateDisplayFormatList.ToList().Count > 0)
			{
				List<int> oCheckedItemList = this.CheckedIssueDateDisplayFormatList.ToList();
				oCheckedItemList.Reverse();
				foreach (int iRowIndex in oCheckedItemList)
				{
					try
					{
						object oValue = this.radGridViewDisplayName.Rows[iRowIndex].Cells["Name"].Value;
						string sValue = (null != oValue) ? oValue.ToString() : string.Empty;
						this.IssueDateDisplayFormatList.Remove(this.IssueDateDisplayFormatList.Find(param => param.Name == sValue));
					}
					catch (Exception ex)
					{
						MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					}
					radGridViewDisplayName.Rows.RemoveAt(iRowIndex);
				}
			}
		}

		/// <summary>
		/// update name
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.08.31</date>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void radGridViewDisplayName_CellEndEdit(object sender, GridViewCellEventArgs e)
		{
			object oValue = this.radGridViewDisplayName.Rows[e.RowIndex].Cells[e.ColumnIndex].Value;
			string sValue = (null != oValue) ? oValue.ToString() : string.Empty;

			var rows = (from GridViewRowInfo row in radGridViewDisplayName.Rows
						where ((null != row.Cells["Name"].Value) && (sValue == row.Cells["Name"].Value.ToString()))
						select row);

			if (rows.ToList().Count > 1)
			{
				MessageBox.Show("There is duplicated name", "Message", MessageBoxButtons.OK, MessageBoxIcon.Information);
				return;
			}

			UserControlRevSetting.IssueDateDisplayFormat oDispFormat = this.radGridViewDisplayName.Rows[e.RowIndex].Tag as UserControlRevSetting.IssueDateDisplayFormat;
			if (null != oDispFormat)
			{
				oDispFormat.Name = sValue;
			}
		}
    }
}
