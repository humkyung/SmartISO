using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL.Entities;
using EDCS.WF.BSL.Enums;
using EDCS.WF.BSL.Helpers;
using EDCS.WF.BSL.SubForms;
using EDCS.WF.BSL.Variable;
using EDCS.WF.DSL;
using Telerik.WinControls;
using Telerik.WinControls.UI;

namespace Configuration
{
    public partial class UserControlEDBUOM : UserControlBase
    {
        string themeName = "Windows8";

        public UserControlEDBUOM()
        {
            try
            {
                this.InitializeComponent();

                this.Dock = DockStyle.Fill;
                this.radGridView1.MouseClick += new MouseEventHandler(radGridView1_MouseClick);
                this.radMenuItem1.Click += new EventHandler(radMenuItem1_Click);
            }
            catch (Exception exception)
            {
                new ErrorHandling(this, exception, this._entityEDBInformation.Employee, this._entityEDBInformation.Project, this._entityEDBInformation.ListType).ErrorMsgDatabaseSave();
            }
        }

        void radMenuItem1_Click(object sender, EventArgs e)
        {
            var entityFrmShareRadGridView = new EntityFrmShareRadGridView()
            {
                EnumFormName = EnumFormName.CommonColumnType,
                Employee = this._entityEDBInformation.Employee,
                Project = this._entityEDBInformation.Project,
                ListType = this._entityEDBInformation.ListType,
            };

            var frmShareRadGridView = new FrmShareRadGridView(entityFrmShareRadGridView);
            if (frmShareRadGridView.ShowDialog().Equals(DialogResult.OK))
            {
                this.LoadData();
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

        private void MethodBindingRadGridView()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                this.radGridView1.BeginUpdate();
                this.radGridView1.Rows.Clear();

                var commonUnits = proxy.ReadCommonUnits(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                foreach (var commonUnit in commonUnits)
                {
                    this.radGridView1.Rows.Add(commonUnit.CommonUnitID, commonUnit.CommonColumnTypeID, commonUnit.Name, commonUnit.NameView);
                }

                //this.radGridView1.GroupDescriptors.Add(new GridGroupByExpression("CommonColumnTypeID as CommonColumnTypeID format \"{0}: {1}\" Group By CommonColumnTypeID"));
                this.radGridView1.MasterTemplate.ExpandAllGroups();
                this.radGridView1.EndUpdate();
            }
        }
        private void MethodSettingRadGridView()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                this.radGridView1.BeginUpdate();
                this.radGridView1.Rows.Clear();
                this.radGridView1.Columns.Clear();
                this.radGridView1.GroupDescriptors.Clear();

                var commonColumnTypes = proxy.ReadCommonColumnTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                commonColumnTypes.Add(new EntityEDB_CommonColumnType() { CommonColumnTypeID = 0, Orders = 0, Name = "", NameView = "" });
                this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "CommonUnitID",
                    IsVisible = false,
                });
                this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "CommonColumnTypeID",
                    HeaderText = "ColumnType",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    DataSource = commonColumnTypes,
                    ValueMember = "CommonColumnTypeID",
                    DisplayMember = "NameView",
                    AutoCompleteMode = AutoCompleteMode.Append,
                    DropDownStyle = RadDropDownStyle.DropDownList,
                });
                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "Name",
                    HeaderText = "Represent",
                });
                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "NameView",
                    HeaderText = "UOM",
                });
                this.MethodBindingRadGridView();

                this.radGridView1.GroupDescriptors.Add("CommonColumnTypeID", ListSortDirection.Descending);
                this.radGridView1.GroupDescriptors.Add(new GridGroupByExpression("Name as Name format \"{0}: {1}\" Group By Name"));

                this.radGridView1.EndUpdate();
            }
        }

        void radGridView1_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                this.radContextMenu1.Show(radGridView1, e.X, e.Y);
            }
        }

        private void MethodDesignForm()
        {
            // radGridView1
            this.radGridView1.AllowAutoSizeColumns = false;
            this.radGridView1.AllowColumnChooser = false;
            this.radGridView1.AllowColumnHeaderContextMenu = false;
            this.radGridView1.AllowColumnReorder = false;
            this.radGridView1.AllowMultiColumnSorting = false;
            this.radGridView1.AllowRowResize = false;
            this.radGridView1.AutoGenerateColumns = false;
            this.radGridView1.AutoSizeColumnsMode = GridViewAutoSizeColumnsMode.Fill;
            this.radGridView1.EnableHotTracking = false;
            this.radGridView1.ShowCellErrors = false;
            this.radGridView1.ShowFilteringRow = false;
            this.radGridView1.ShowNoDataText = false;
            this.radGridView1.ShowRowErrors = false;
            this.radGridView1.TableElement.TableHeaderHeight = 20;
            this.radGridView1.TableElement.RowHeight = 21;
            this.radGridView1.MasterTemplate.ShowRowHeaderColumn = false;
            this.radGridView1.MasterTemplate.ShowHeaderCellButtons = false;

            this.radGridView1.Font = new Font("Tahoma", (float)8.25, FontStyle.Regular);

            this.radGridView1.SelectionMode = GridViewSelectionMode.CellSelect;

            this.radGridView1.ShowGroupPanel = false;
            //
            this.radGridView1.MasterTemplate.EnableFiltering = true;
            this.radGridView1.MasterTemplate.AutoExpandGroups = true;
            this.radGridView1.EnableHotTracking = true;
            this.radGridView1.MasterTemplate.EnableFiltering = true;
            this.radGridView1.MasterTemplate.ShowFilteringRow = false;
            this.radGridView1.MasterTemplate.ShowGroupedColumns = true;
            this.radGridView1.AllowCellContextMenu = false;
            //this.radGridView1.MultiSelect = true;
            //this.radGridView1.EnableGrouping = false;
        }
        private void MethodSaveShareRadGridView()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var commonUnits = new List<EDB_CommonUnit>();

                foreach (var row in this.radGridView1.Rows.OrderBy(ob => ob.Index))
                {
                    var commonUnit = new EDB_CommonUnit();

                    var commonColumnTypeID = (int?)row.Cells["CommonColumnTypeID"].Value;
                    commonUnit.CommonUnitID = Convert.ToInt32(row.Cells["CommonUnitID"].Value);
                    commonUnit.Orders = (row.Index + 1) * 100;
                    commonUnit.CommonColumnTypeID = commonColumnTypeID == 0 ? null : commonColumnTypeID;
                    commonUnit.Name = row.Cells["Name"].Value != null ? row.Cells["Name"].Value.ToString().Trim() : "";
                    commonUnit.NameView = row.Cells["NameView"].Value != null ? row.Cells["NameView"].Value.ToString().Trim() : "";

                    commonUnits.Add(commonUnit);
                }

                proxy.CreateCommonUnit(EDCS.WF.BSL.Variable.SharedVariable.IsIE, commonUnits);
            }
        }
    }
}
