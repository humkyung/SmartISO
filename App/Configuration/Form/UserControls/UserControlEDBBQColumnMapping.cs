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
using Telerik.WinControls.Data;
using Telerik.WinControls.UI;

namespace Configuration
{
    public partial class UserControlEDBBQColumnMapping : UserControlBase
    {
        public List<EntityEDB_CommonColumnDynamic> _commonColumnDynamics;
        public List<EntityEDB_BQColumnMapping> _oldBQColumnMappings;
        public List<EntityEDB_BQColumnMapping> _entityBQColumnMappings;
        public int _prevSelectedID;
        public int _originIndex;
        public int _lastIndex;

        public UserControlEDBBQColumnMapping()
        {
            try
            {
                this.InitializeComponent();
                this.Dock = DockStyle.Fill;

                // General
                //this.MethodDesignForm();
                this.MethodDesignForm();

            }
            catch (Exception exception)
            {
                new ErrorHandling(this, exception, this._entityEDBInformation.Employee, this._entityEDBInformation.Project, this._entityEDBInformation.ListType).ErrorMsgDatabaseSave();
            }
        }

        public override void LoadData()
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    this.radDropDownListListType.Enabled = false;
                    this.radDropDownListListType.ValueMember = "ListTypeID";
                    this.radDropDownListListType.DisplayMember = "ComplexNameView";
                    this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                    this.radDropDownListListType.SelectedValue = 4;
                    this.MethodSettingRadGridView();
                }
            }
            catch (System.ServiceModel.FaultException ex)
            {
                
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
            catch (Exception ex)
            {
                
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }


        }
        public override void SaveData()
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    if (this.radGridView4.Rows.Count > 0)
                    {
                        foreach (var row in this.radGridView4.Rows)
                        {
                            var bqColumnMappingID = Convert.ToInt32(row.Cells["BQColumnMappingID"].Value);
                            var entityBQColumnMapping = this._oldBQColumnMappings.SingleOrDefault(s => s.BQColumnMappingID == bqColumnMappingID);

                            entityBQColumnMapping.ColumnLabelLevel3 = row.Cells["ColumnLabelLevel3"].Value as string;
                            entityBQColumnMapping.ColumnLabelLevel4 = row.Cells["ColumnLabelLevel4"].Value as string;
                            entityBQColumnMapping.ColumnLabelLevel5 = row.Cells["ColumnLabelLevel5"].Value as string;

                            entityBQColumnMapping.BQMappingTypeLevel3 = Convert.ToInt32(row.Cells["BQMappingTypeLevel3"].Value);
                            entityBQColumnMapping.BQMappingTypeLevel4 = Convert.ToInt32(row.Cells["BQMappingTypeLevel4"].Value);
                            entityBQColumnMapping.BQMappingTypeLevel5 = Convert.ToInt32(row.Cells["BQMappingTypeLevel5"].Value);
                        }
                    }
                    proxy.UpdateBQColumnMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, this._oldBQColumnMappings);
                }
            }
            catch (System.ServiceModel.FaultException ex)
            {
                
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }
            catch (Exception ex)
            {
                
                new TechSun.Theme.frmWarning(ex.Message.ToString()).ShowDialog(this);
            }

        }
        private void MethodSettingRadGridView()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                this.radGridView1.Rows.Clear();
                this.radGridView1.Columns.Clear();

                this.radGridView1.GridViewElement.TableElement.TableHeaderHeight = 25;
                this._commonColumnDynamics = proxy.ReadCommonColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, 4);
                this._commonColumnDynamics.Insert(0, new EntityEDB_CommonColumnDynamic() { Name = "", NameView = "" });
                this._oldBQColumnMappings = proxy.ReadBQColumnMappings(EDCS.WF.BSL.Variable.SharedVariable.IsIE, 4);
                this._entityBQColumnMappings = proxy.ReadBQColumnMappings(EDCS.WF.BSL.Variable.SharedVariable.IsIE, 4);
                var equipmentTypeDynamicDefaults = proxy.ReadEquipmentTypeDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);

                foreach (var mapping in this._oldBQColumnMappings)
                {
                    var type = equipmentTypeDynamicDefaults.SingleOrDefault(s => s.Name == mapping.GroupCode + mapping.EquipmentTypeName);
                    var subType = equipmentTypeDynamicDefaults.SingleOrDefault(s => s.Name == mapping.GroupCode + mapping.EquipmentTypeName + mapping.EquipmentTypeSubName);
                    if (type != null)
                    {
                        mapping.EquipmentTypeNameView = type.NameView;
                    }
                    if (subType != null)
                    {
                        mapping.EquipmentTypeSubNameView = subType.NameView;
                    }
                }
                // radGridView1 Setting
                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "GroupCode",
                    HeaderText = "GroupCode",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //IsVisible = false,
                    //ReadOnly = this._entityFrmEquipmentTypeDynamic.ListType.Name.Equals(Enum.GetName(typeof(EDCS.WF.BSL.Enums.EnumNameCheck), EDCS.WF.BSL.Enums.EnumNameCheck.PowerEquipmentList))
                    //? false : true
                });
                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "Name",
                    HeaderText = "Key",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    MaxLength = 3,
                    //IsVisible = false,
                    ReadOnly = true,
                    //ReadOnly = this._entityFrmEquipmentTypeDynamic.ListType.Name.Equals(Enum.GetName(typeof(EDCS.WF.BSL.Enums.EnumNameCheck), EDCS.WF.BSL.Enums.EnumNameCheck.PowerEquipmentList))
                    //? false : true
                });
                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "NameView",
                    HeaderText = "Eq. Type Name",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    MaxLength = 25,
                    AllowSort = false,
                    ReadOnly = true,

                    // ReadOnly = this._entityFrmEquipmentTypeDynamic.ListType.Name.Equals(Enum.GetName(typeof(EDCS.WF.BSL.Enums.EnumNameCheck), EDCS.WF.BSL.Enums.EnumNameCheck.PowerEquipmentList))
                    //? false : true
                });

                this.radGridView4.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "BQColumnMappingID",
                    IsVisible = false,
                });
                this.radGridView4.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "GroupCode",
                    HeaderText = "Discipline",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    MaxLength = 3,
                    IsVisible = false,
                });
                this.radGridView4.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "Name",
                    HeaderText = "Key",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    MaxLength = 3,
                });
                this.radGridView4.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "NameView",
                    HeaderText = "Eq. Type Name",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    MaxLength = 25,
                    // ReadOnly = this._entityFrmEquipmentTypeDynamic.ListType.Name.Equals(Enum.GetName(typeof(EDCS.WF.BSL.Enums.EnumNameCheck), EDCS.WF.BSL.Enums.EnumNameCheck.PowerEquipmentList))
                    //? false : true
                    ReadOnly = true,
                });

                this.radGridView4.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "ColumnLabelLevel3",
                    HeaderText = "Column Level3",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    DataSource = this._commonColumnDynamics,
                    DisplayMember = "NameView",
                    ValueMember = "Name",
                });
                var BQMappingTypeTable = new DataTable("Table1");
                BQMappingTypeTable.Columns.Add("Value", typeof(int));
                BQMappingTypeTable.Columns.Add("Text", typeof(string));
                BQMappingTypeTable.Rows.Add(0, "");
                foreach (EnumBQMappingType type in Enum.GetValues(typeof(EnumBQMappingType)))
                {
                    BQMappingTypeTable.Rows.Add((int)type, type.ToString());
                }
                this.radGridView4.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "BQMappingTypeLevel3",
                    HeaderText = "Type Level3",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    DataSource = BQMappingTypeTable,
                    ValueMember = "Value",
                    DisplayMember = "Text",
                    DropDownStyle = RadDropDownStyle.DropDownList,
                });

                this.radGridView4.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "ColumnLabelLevel4",
                    HeaderText = "Column Level4",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    DataSource = this._commonColumnDynamics,
                    DisplayMember = "NameView",
                    ValueMember = "Name",
                });

                this.radGridView4.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "BQMappingTypeLevel4",
                    HeaderText = "Type Level4",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    DataSource = BQMappingTypeTable,
                    ValueMember = "Value",
                    DisplayMember = "Text",
                    DropDownStyle = RadDropDownStyle.DropDownList,
                });
                this.radGridView4.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "ColumnLabelLevel5",
                    HeaderText = "Column Level5",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    DataSource = this._commonColumnDynamics,
                    DisplayMember = "NameView",
                    ValueMember = "Name",
                });

                this.radGridView4.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "BQMappingTypeLevel5",
                    HeaderText = "Type Level5",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    DataSource = BQMappingTypeTable,
                    ValueMember = "Value",
                    DisplayMember = "Text",
                    DropDownStyle = RadDropDownStyle.DropDownList,
                });
                foreach (var mapping in this._oldBQColumnMappings.Distinct())
                {
                    this.radGridView1.Rows.Add(mapping.GroupCode, mapping.EquipmentTypeName, mapping.EquipmentTypeNameView);
                }

                this.radGridView1.SelectionChanged += new EventHandler(radGridView1_SelectionChanged);

                this.radGridView1.CurrentRow = null;
                if (this.radGridView1.Rows.Count > 0)
                {
                    this.radGridView1.CurrentRow = this.radGridView1.Rows[0];
                }

                //this.MethodBindingRadGridView();
            }
        }
        private void MethodBindingRadGridView(
        int p_originIndex = 0
        , int p_lastIndex = 1)
        {
            this.radGridView1.SelectionChanged -= new EventHandler(radGridView1_SelectionChanged);
            this.radGridView1.RowsChanged -= new GridViewCollectionChangedEventHandler(radGridView1_RowsChanged);

            this._prevSelectedID = -1;
            this._originIndex = p_originIndex;
            this._lastIndex = p_lastIndex;
            //this.radGridView1.Rows.Clear();


            this.radGridView1.SelectionChanged += new EventHandler(radGridView1_SelectionChanged);
            this.radGridView1.RowsChanged += new GridViewCollectionChangedEventHandler(radGridView1_RowsChanged);
            if (this.radGridView1.Rows.Count > 0)
            {
                this.radGridView1.CurrentRow = null;
                this.radGridView1.CurrentRow = this.radGridView1.Rows[0];
            }

        }
        private void MethodSaveShareRadGridView(bool p_flagSaveAfterClose, bool p_flagDialogResult)
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                if (this.radGridView4.Rows.Count > 0)
                {
                    foreach (var row in this.radGridView4.Rows)
                    {
                        var bqColumnMappingID = Convert.ToInt32(row.Cells["BQColumnMappingID"].Value);
                        var entityBQColumnMapping = this._oldBQColumnMappings.SingleOrDefault(s => s.BQColumnMappingID == bqColumnMappingID);

                        entityBQColumnMapping.ColumnLabelLevel3 = row.Cells["ColumnLabelLevel3"].Value as string;
                        entityBQColumnMapping.ColumnLabelLevel4 = row.Cells["ColumnLabelLevel4"].Value as string;
                        entityBQColumnMapping.ColumnLabelLevel5 = row.Cells["ColumnLabelLevel5"].Value as string;

                        entityBQColumnMapping.BQMappingTypeLevel3 = Convert.ToInt32(row.Cells["BQMappingTypeLevel3"].Value);
                        entityBQColumnMapping.BQMappingTypeLevel4 = Convert.ToInt32(row.Cells["BQMappingTypeLevel4"].Value);
                        entityBQColumnMapping.BQMappingTypeLevel5 = Convert.ToInt32(row.Cells["BQMappingTypeLevel5"].Value);
                    }
                }

                proxy.UpdateBQColumnMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, this._oldBQColumnMappings);
            }
        }
        private void MethodDesignForm()
        {

            this.radLabel2.Font = new Font("Tahoma", 9F, FontStyle.Bold);

            this.radLabel2.ForeColor = Color.DarkSlateGray;
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
            this.radGridView1.TableElement.TableHeaderHeight = 40;
            this.radGridView1.TableElement.RowHeight = 30;
            //
            this.radGridView1.MultiSelect = true;
            this.radGridView1.EnableGrouping = false;
            this.radGridView1.AllowAddNewRow = false;
            this.radGridView1.AllowDeleteRow = false;
            this.radGridView1.AllowRowReorder = false;

            this.radGridView4.AllowMultiColumnSorting = false;
            this.radGridView4.AllowRowResize = false;
            this.radGridView4.AutoGenerateColumns = false;
            this.radGridView4.AutoSizeColumnsMode = GridViewAutoSizeColumnsMode.Fill;
            this.radGridView4.EnableHotTracking = false;
            this.radGridView4.ShowCellErrors = false;
            this.radGridView4.ShowFilteringRow = false;
            this.radGridView4.ShowNoDataText = false;
            this.radGridView4.ShowRowErrors = false;
            this.radGridView4.AllowRowReorder = false;
            this.radGridView4.MasterTemplate.AutoGenerateColumns = false;
            this.radGridView4.MasterTemplate.AllowEditRow = true;
            this.radGridView4.MasterTemplate.AddNewBoundRowBeforeEdit = true;
            this.radGridView4.AllowDragToGroup = false;
            this.radGridView4.AllowCellContextMenu = false;
            this.radGridView4.EnableGrouping = false;
            this.radGridView4.AllowColumnHeaderContextMenu = false;
            //
            this.radGridView4.AllowAddNewRow = false;
            this.radGridView4.AllowDeleteRow = false;

            this.radGridView1.AllowAddNewRow = false;
            this.radGridView1.AllowDeleteRow = false;
            this.radGridView1.AllowEditRow = false;
            //this.radGridView4.AllowEditRow = false;

            this.Size = new Size(1000, 800);
            this.Text = "BQ Mapping";
            //this.radGridView1.AllowRowReorder = true;
            this.radGridView1.AllowRowReorder = false;

            this.radGridView1.AllowAddNewRow = false;
            this.radGridView1.AllowDeleteRow = false;
            this.radGridView1.AllowEditRow = false;
            this.radGridView4.AllowAddNewRow = false;
            this.radGridView4.AllowDeleteRow = false;

        }
        private void radGridView1_CellValidating(object sender, CellValidatingEventArgs e)
        {
            if (e.ColumnIndex > 0 || e.RowIndex > -1) // Delete 아닐 때 (ColumnIndex:-1, RowIndex:-1), 첫번째 Column 제외 (ID값은 추가시 자동으로 부여되기 때문에 없어야 함)
            {
                if (e.ColumnIndex > 0) // Reload 아닐 때 (ColumnIndex:-1), 첫번째 Column 제외 (ID값은 추가시 자동으로 부여되기 때문에 없어야 함)
                {
                    if (e.RowIndex > -1) // Insert 아닐 때(RowIndex:-1)
                    {
                        if (!e.Column.Name.Equals("NameKor")) // NameKor 필드
                        {
                            if (e.Value == null || string.IsNullOrWhiteSpace(e.Value.ToString())) // 공백 검사
                            {
                                e.Cancel = true;
                                new CustomRadMessageBoxShowAndTheme("Please fill in all the details without blanks.").RadMessageBoxShow();
                            } // end if
                        } // end if
                    } // end if
                }
            }
        }
        private void radGridView1_SelectionChanged(object sender, EventArgs e)
        {
            if (this.radGridView1.SelectedRows.Count > 0)
            {
                if (this.radGridView4.Rows.Count > 0)
                {
                    foreach (var row in this.radGridView4.Rows)
                    {
                        var bqColumnMappingID = Convert.ToInt32(row.Cells["BQColumnMappingID"].Value);
                        var entityBQColumnMapping = this._oldBQColumnMappings.SingleOrDefault(s => s.BQColumnMappingID == bqColumnMappingID);

                        entityBQColumnMapping.ColumnLabelLevel3 = row.Cells["ColumnLabelLevel3"].Value as string;
                        entityBQColumnMapping.ColumnLabelLevel4 = row.Cells["ColumnLabelLevel4"].Value as string;
                        entityBQColumnMapping.ColumnLabelLevel5 = row.Cells["ColumnLabelLevel5"].Value as string;

                        entityBQColumnMapping.BQMappingTypeLevel3 = Convert.ToInt32(row.Cells["BQMappingTypeLevel3"].Value);
                        entityBQColumnMapping.BQMappingTypeLevel4 = Convert.ToInt32(row.Cells["BQMappingTypeLevel4"].Value);
                        entityBQColumnMapping.BQMappingTypeLevel5 = Convert.ToInt32(row.Cells["BQMappingTypeLevel5"].Value);
                    }
                    this.radGridView4.Rows.Clear();
                }
                var equipmentTypeGroupCode = this.radGridView1.SelectedRows[0].Cells["GroupCode"].Value as string;
                var equipmentTypeName = this.radGridView1.SelectedRows[0].Cells["Name"].Value as string;

                var selectedEquipmentTypeSub = this._oldBQColumnMappings.Where(w => w.GroupCode == equipmentTypeGroupCode && w.EquipmentTypeName == equipmentTypeName);

                foreach (var equipmentType in selectedEquipmentTypeSub)
                {
                    this.radGridView4.Rows.Add(equipmentType.BQColumnMappingID, equipmentType.GroupCode, equipmentType.EquipmentTypeSubName, equipmentType.EquipmentTypeSubNameView, equipmentType.ColumnLabelLevel3, equipmentType.BQMappingTypeLevel3, equipmentType.ColumnLabelLevel4, equipmentType.BQMappingTypeLevel4, equipmentType.ColumnLabelLevel5, equipmentType.BQMappingTypeLevel5);
                }

                //this._prevSelectedID = Convert.ToInt32(this.radGridView1.SelectedRows[0].Cells["EquipmentTypeDynamicID"].Value);
            }
        }
        private void radGridView1_RowsChanged(object sender, GridViewCollectionChangedEventArgs e)
        {
            if (e.Action == NotifyCollectionChangedAction.Add)
            {
                this.radGridView1.Rows[e.NewStartingIndex].Cells["EquipmentTypeDynamicID"].Value = this._lastIndex;
                this._lastIndex++;
            }
            else if (e.Action == NotifyCollectionChangedAction.Remove)
            {
                this._prevSelectedID = -1;
            }
        }

    }
}
