using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL.Enums;
using EDCS.WF.BSL.Helpers;
using EDCS.WF.BSL.Variable;
using EDCS.WF.DSL;
using Telerik.WinControls;
using Telerik.WinControls.UI;
using EDCS.WF.BSL.Method;
using CommonLibrary.CL;

namespace Configuration
{
    public partial class UserControlCommonEmployee : UserControlBase
    {
        string themeName = "Windows8";

        int ListTypeInstrumentProcessData = 3;
        int ListTypeInstrument3DDimension = 18;

        private List<EntityCommon_Employee> _employees;
        string[] searchColumnName;

        public UserControlCommonEmployee()
        {
            try
            {
                this.InitializeComponent();

                this.radGridView1.CustomFiltering += new GridViewCustomFilteringEventHandler(radGridView1_CustomFiltering);
                this.radTextBox1.TextChanged += new EventHandler(radTextBox1_TextChanged);

                this.Dock = DockStyle.Fill;
            }
            catch (Exception exception)
            {
                new ErrorHandling(this, exception, this._entityEDBInformation.Employee, this._entityEDBInformation.Project, this._entityEDBInformation.ListType).ErrorMsgDatabaseSave();
            }
        }

        void radGridView1_CustomFiltering(object sender, GridViewCustomFilteringEventArgs e)
        {
            if (string.IsNullOrWhiteSpace(this.radTextBox1.Text))
            {
                this.radGridView1.BeginUpdate();
                e.Visible = true;
                for (int i = 0; i < this.radGridView1.ColumnCount; i++)
                {
                    e.Row.Cells[i].Style.Reset();
                }
                this.radGridView1.EndUpdate(false);
                return;
            }
            this.radGridView1.BeginUpdate();
            e.Visible = false;
            for (int i = 0; i < this.radGridView1.ColumnCount; i++)
            {
                if (searchColumnName.Contains(this.radGridView1.Columns[i].Name))
                {
                    string text = e.Row.Cells[i].Value.ToString().ToUpper();
                    //if (text.IndexOf(this.filterTextBox.Text, 0, StringComparison.InvariantCultureIgnoreCase) >= 0)
                    if (text.Contains(this.radTextBox1.Text.ToUpper()))
                    {
                        e.Visible = true;
                        e.Row.Cells[i].Style.CustomizeFill = true;
                        e.Row.Cells[i].Style.DrawFill = true;
                        e.Row.Cells[i].Style.BackColor = Color.FromArgb(201, 252, 254);
                    }
                    else
                    {
                        e.Row.Cells[i].Style.Reset();
                    }
                    this.radGridView1.EndUpdate(false);
                }
            }
        }

        public override void LoadData()
        {
            this.MethodDesignForm();
            this.MethodSettingRadGridView();
        }
        public override void SaveData()
        {
            this.MethodSaveShareRadGridView();
        }

        void radTextBox1_TextChanged(object sender, EventArgs e)
        {
            this.radGridView1.MasterTemplate.Refresh();
        }

        private void MethodSettingRadGridView()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                this.radGridView1.BeginUpdate();
                //this.radGridView1.Rows.Clear();

                var authorityTable = new DataTable("Table1");
                authorityTable.Columns.Add("Value", typeof(int));
                authorityTable.Columns.Add("Text", typeof(string));
                foreach (EnumAuthorityType type in Enum.GetValues(typeof(EnumAuthorityType)))
                {
                    authorityTable.Rows.Add((int)type, type.ToString());
                }
                // Columns Setting
                this.radGridView1.Columns.Add(new GridViewCheckBoxColumn()
                {
                    Name = "DeleteCheck",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    IsVisible = true,
                    Width = 24,
                    MinWidth = 24,
                    MaxWidth = 24,
                    EnableHeaderCheckBox = true,
                });
                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "EmployeeID",
                    IsVisible = false,
                });
                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "No",
                    HeaderText = "EmployeeNo",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    MaxLength = 30,
                    Width = 120
                });

                this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "Authority",
                    HeaderText = "Authority",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    DataSource = authorityTable,
                    ValueMember = "Value",
                    DisplayMember = "Text",
                    AutoCompleteMode = AutoCompleteMode.Append,
                    DropDownStyle = RadDropDownStyle.DropDown,
                    IsVisible = false,
                    Width = 0
                });
                this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "DisciplineID",
                    HeaderText = "Department",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    DataSource = proxy.ReadDisciplines(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false),
                    ValueMember = "DisciplineID",
                    DisplayMember = "NameView",
                    AutoCompleteMode = AutoCompleteMode.Append,
                    DropDownStyle = RadDropDownStyle.DropDown,
                    IsVisible = true,
                    Width = 120
                });


                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "GroupCode",
                    HeaderText = "Group Code",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    MaxLength = 100,
                    ReadOnly = true,
                    IsVisible = false,
                    Width = 0
                });

                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "GroupName",
                    HeaderText = "Departement",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    MaxLength = 100,
                    ReadOnly = true,
                    Width = 120

                });

                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "NameKor",
                    HeaderText = "Name(Kor)",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    MaxLength = 100,
                    Width = 120
                });

                this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "RoleID",
                    HeaderText = "Role",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    DataSource = proxy.ReadCommon_Role(EDCS.WF.BSL.Variable.SharedVariable.IsIE).Where(w => w.DeleteFlag == false).ToList(),
                    ValueMember = "RoleID",
                    DisplayMember = "Name",
                    AutoCompleteMode = AutoCompleteMode.Append,
                    DropDownStyle = RadDropDownStyle.DropDown,
                    Width = 120
                });

                this.radGridView1.Columns.Add(new GridViewCheckBoxColumn()
                {
                    Name = "ExternalExternalUserFlag",
                    HeaderText = "External",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    IsVisible = true,
                    Width = 80,
                    MinWidth = 80,
                    MaxWidth = 80,
                    EnableHeaderCheckBox = true,
                });

                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "ExternalUserPwd",
                    HeaderText = "External User Password",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    MaxLength = 100,
                    Width = 120
                });

                //this.radGridView1.Columns.Add(new GridViewCheckBoxColumn()
                //{
                //    Name = "ExternalUserFlag",
                //    HeaderText = "External User",
                //    DataType = typeof(bool),
                //    TextAlignment = ContentAlignment.MiddleCenter
                //});

                List<EntityGroupInfo> entityGroupInfos = new List<EntityGroupInfo>();
                var userList = new List<EntityEDB_UserInfo>();
                proxy.ReadInsaUserInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref userList);

                proxy.ReadInsaGroupInfo(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref entityGroupInfos);
                userList.Where(w => string.IsNullOrWhiteSpace(w.Email)).ToList().ConvertAll(ca => ca.Email = ca.UserID + "@daelim.co.kr");
                foreach (var entityUserInfo in userList.GroupBy(g => g.GroupCode))
                {
                    var entityGroupInfo = entityGroupInfos.FirstOrDefault(s => s.GroupCode == entityUserInfo.Key);
                    if (entityGroupInfo != null)
                    {
                        entityUserInfo.ToList().ForEach(f => f.GroupName = entityGroupInfo.GroupName);
                    }
                }

                //var employees = proxy.ReadEmployees(false)
                this._employees = proxy.ReadEmployees(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false)
                    //.Where(w => w.VisibleFlag.Equals(true))
                    .ToList();

                // 외부 사용자 추가
                this._employees.ConvertAll(ca => ca.ExternalUserPwd = !ca.ExternalUserFlag ? "" : CommonLibrary.Base64EncodeDecode.Base64Decode(ca.ExternalUserPwd));

                this._employees.ConvertAll(ca => ca.DepartmentNameView = userList.FirstOrDefault(f => f.UserID == ca.No) == null ? "" : userList.FirstOrDefault(f => f.UserID == ca.No).GroupName);
                this._employees.ConvertAll(ca => ca.DepartmentName = userList.FirstOrDefault(f => f.UserID == ca.No) == null ? "" : userList.FirstOrDefault(f => f.UserID == ca.No).GroupCode);

                searchColumnName = new string[] { "No", "NameKor", "GroupName" };
                foreach (var employee in this._employees)
                {
                    this.radGridView1.Rows.Add(false, employee.EmployeeID, employee.No, employee.Authority, employee.DisciplineID, employee.DepartmentName, employee.DepartmentNameView, employee.NameKor, employee.RoleID, employee.ExternalUserFlag, employee.ExternalUserPwd);
                }
                this.radGridView1.EndUpdate();
                this.radTextBox1.Focus();
            }
        }
        private void MethodDesignForm()
        {
            // radGridView1
            this.radGridView1.AllowMultiColumnSorting = false;
            this.radGridView1.AllowRowResize = false;
            this.radGridView1.AutoGenerateColumns = false;
            this.radGridView1.AutoSizeColumnsMode = GridViewAutoSizeColumnsMode.Fill;
            this.radGridView1.ShowCellErrors = false;
            this.radGridView1.ShowFilteringRow = false;
            this.radGridView1.ShowNoDataText = false;
            this.radGridView1.ShowRowErrors = false;
            this.radGridView1.AllowRowReorder = false;
            this.radGridView1.MasterTemplate.AutoGenerateColumns = false;
            this.radGridView1.MasterTemplate.AllowEditRow = true;
            this.radGridView1.MasterTemplate.AddNewBoundRowBeforeEdit = true;
            this.radGridView1.AllowDragToGroup = false;
            this.radGridView1.AllowCellContextMenu = false;
            this.radGridView1.EnableGrouping = false;
            this.radGridView1.AllowColumnHeaderContextMenu = false;
            this.radGridView1.AllowAddNewRow = false;
            this.radGridView1.MultiSelect = false;
            this.radGridView1.AllowRowReorder = true;
            this.radGridView1.MasterTemplate.AllowEditRow = true;
            //this.radGridView1.AllowDeleteRow = true;
            this.radGridView1.MasterTemplate.AutoGenerateColumns = false;

            this.radGridView1.Font = new Font("Tahoma", (float)8.25, FontStyle.Regular);

            this.radGridView1.SelectionMode = GridViewSelectionMode.FullRowSelect;
            this.radGridView1.MultiSelect = true;
            //
            this.radGridView1.EnableFiltering = true;
            this.radGridView1.EnableCustomFiltering = true;
            this.radGridView1.EnableHotTracking = true;
            this.radGridView1.ShowFilteringRow = false;


            this.radGridView1.EnableGrouping = false;
            this.radGridView1.AllowCellContextMenu = false;
        }
        private void MethodSaveShareRadGridView()
        {

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var employees = new List<Common_Employee>();

                foreach (var row in this.radGridView1.Rows.OrderBy(ob => ob.Index))
                {
                    var employee = new Common_Employee();

                    employee.EmployeeID = Convert.ToInt32(row.Cells["EmployeeID"].Value);

                    employee.Orders = (row.Index + 1) * 100;
                    //employee.Authority = Convert.ToByte(row.Cells["Authority"].Value);
                    employee.DisciplineID = row.Cells["DisciplineID"].Value == null ? 18 : Convert.ToInt32(row.Cells["DisciplineID"].Value);
                    employee.No = row.Cells["No"].Value != null ? row.Cells["No"].Value.ToString().Trim() : null;
                    employee.NameKor = row.Cells["NameKor"].Value != null ? row.Cells["NameKor"].Value.ToString().Trim() : "";
                    employee.RoleID = Convert.ToInt64(row.Cells["RoleID"].Value);

                    if (row.Cells["ExternalExternalUserFlag"].Value != null && (bool)row.Cells["ExternalExternalUserFlag"].Value == true)
                    {
                        employee.ExternalUserFlag = true;
                        employee.ExternalUserPwd = CommonLibrary.Base64EncodeDecode.Base64Encode(row.Cells["ExternalUserPwd"].Value.ToString().Trim());
                        //employee.ExternalUserPwd = new EncryptDecryptQueryString().Encrypt(row.Cells["ExternalUserPwd"].Value.ToString().Trim(), "r0b1nr0y"); 
                    }
                    else
                    {
                        employee.ExternalUserFlag = false;
                        employee.ExternalUserPwd = string.Empty;
                    }

                    employees.Add(employee);
                }

                if (this._employees.Count(c => string.IsNullOrWhiteSpace(c.No)) > 0
                     || this._employees.Count(c => string.IsNullOrWhiteSpace(c.NameKor)) > 0
                     || this._employees.Count(c => c.DisciplineID == 0) > 0)
                {
                    new CustomRadMessageBoxShowAndTheme("Please fill in all the details without blanks.").RadMessageBoxShow();
                    return;
                }

                proxy.CreateEmployees(EDCS.WF.BSL.Variable.SharedVariable.IsIE, employees);
            }

        }

        private void radButtonInsertRow_Click(object sender, EventArgs e)
        {
            GridViewRowInfo oGridRow = this.radGridView1.Rows.AddNew();
        }

        private void radButtonRemoveRow_Click(object sender, EventArgs e)
        {
            for (int i = this.radGridView1.Rows.Count - 1; i > -1; i--)
            {
                if (this.radGridView1.Rows[i].Cells["DeleteCheck"].Value != null && (bool)this.radGridView1.Rows[i].Cells["DeleteCheck"].Value == true)
                {
                    this.radGridView1.Rows.RemoveAt(i);
                }
            }
        }
    }
}
