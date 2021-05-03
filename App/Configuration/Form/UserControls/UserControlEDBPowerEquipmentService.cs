using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL.Helpers;
using EDCS.WF.BSL.Method;
using EDCS.WF.BSL.Variable;
using EDCS.WF.DSL;
using Telerik.WinControls;
using Telerik.WinControls.UI;

namespace Configuration
{
    public partial class UserControlEDBPowerEquipmentService : UserControlBase
    {
        public int _prevSelectedID;
        public int _originIndex;
        public int _lastIndex;
        public List<EntityEDB_PowerEquipmentService> _entityPowerEquipmentServices;
        public List<EntityEDB_CommonSystemDynamic> _oldCommonSystemDynamics;
        public UserControlEDBPowerEquipmentService()
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

                    this.radDropDownListProject.ValueMember = "ProjectID";
                    this.radDropDownListProject.DisplayMember = "NameView";
                    this.radDropDownListProject.DataSource = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true).ToList();
                    this.radDropDownListProject.SelectedValue = this._entityEDBInformation.Project.ProjectID;

                    this.radDropDownListListType.ValueMember = "ListTypeID";
                    this.radDropDownListListType.DisplayMember = "ComplexNameView";
                    this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                    this.radDropDownListListType.SelectedValue = 5;

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
        }
        private void MethodSavePreviousPowerEquipmentService()
        {
            //////////////// 이전 선택된것 저장.
            if (!this._prevSelectedID.Equals(-1))
            {

                SharedMethod.MethodCreateName(this.radGridView2, "Name", "NameView");
                //Debug.WriteLine("Count1 {0}, {1}", this._entityMultiProperties.Count, this._prevSelectedID);
                this._entityPowerEquipmentServices.RemoveAll(ra => ra.CommonSystemDynamicID.Equals(this._prevSelectedID));
                //Debug.WriteLine("Count2 {0}", this._entityMultiProperties.Count);
                foreach (var row in this.radGridView2.Rows.OrderBy(ob => ob.Index))
                {
                    if (row.Cells["Name"].Value == null || row.Cells["NameView"].Value == null || row.Cells["Name"].Value.ToString().Trim() == "" || row.Cells["NameView"].Value.ToString().Trim() == "")
                    {
                        continue;
                    }
                    var powerEquipmentService = new EntityEDB_PowerEquipmentService();
                    powerEquipmentService.PowerEquipmentServiceID = Convert.ToInt32(row.Cells["PowerEquipmentServiceID"].Value);
                    powerEquipmentService.Name = row.Cells["Name"].Value.ToString().Trim();
                    powerEquipmentService.NameView = row.Cells["NameView"].Value.ToString().Trim();
                    //DH EquipmentType
                    powerEquipmentService.CommonSystemDynamicID = this._prevSelectedID;
                    powerEquipmentService.Orders = (row.Index + 1) * 100;
                    this._entityPowerEquipmentServices.Add(powerEquipmentService);
                }
            }
            ////////////////
        }
   
        private void MethodDesignForm()
        {
            // radDropDownList1
            this.radDropDownListProject.ListElement.Font = new Font("Tahoma", 8.25F, FontStyle.Bold);
            this.radDropDownListListType.ListElement.Font = new Font("Tahoma", 8.25F, FontStyle.Bold);

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
            this.radGridView1.ReadOnly = true;

            this.radGridView2.AllowMultiColumnSorting = false;
            this.radGridView2.AllowRowResize = false;
            this.radGridView2.AutoGenerateColumns = false;
            this.radGridView2.AutoSizeColumnsMode = GridViewAutoSizeColumnsMode.Fill;
            this.radGridView2.EnableHotTracking = false;
            this.radGridView2.ShowCellErrors = false;
            this.radGridView2.ShowFilteringRow = false;
            this.radGridView2.ShowNoDataText = false;
            this.radGridView2.ShowRowErrors = false;
            this.radGridView2.AllowRowReorder = true;
            this.radGridView2.MasterTemplate.AutoGenerateColumns = false;
            this.radGridView2.MasterTemplate.AllowEditRow = true;
            this.radGridView2.MasterTemplate.AddNewBoundRowBeforeEdit = true;
            this.radGridView2.AllowDragToGroup = false;
            this.radGridView2.AllowCellContextMenu = false;
            this.radGridView2.EnableGrouping = false;
            this.radGridView2.AllowColumnHeaderContextMenu = false;
        }

        private void MethodBindingRadGridView(
           int p_originIndex = 0
           , int p_lastIndex = 1
           , List<EntityEDB_CommonSystemDynamic> p_entityCommonSystemDynamics = null
           , List<EntityEDB_PowerEquipmentService> p_entityPowerEquipmentServices = null)
        {
            this.radGridView1.SelectionChanged -= new EventHandler(radGridView1_SelectionChanged);
            this.radGridView1.SelectionChanging -= radGridView1_SelectionChanging;
            this._prevSelectedID = -1;
            this._originIndex = p_originIndex;
            this._lastIndex = p_lastIndex;
            this.radGridView1.Rows.Clear();
            List<EntityEDB_CommonSystemDynamic> commonSystemDynamics = null;

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                if (p_entityCommonSystemDynamics != null)
                {
                    commonSystemDynamics = p_entityCommonSystemDynamics;
                }
                else
                {
                    commonSystemDynamics = proxy.ReadCommonSystemDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                }
                // Rows Setting
                foreach (var commonSystemDynamic in commonSystemDynamics)
                {
                    //this.radGridView1.Rows.Add(equipmentTypeDynamic.EquipmentTypeDynamicID, equipmentTypeDynamic.Name, equipmentTypeDynamic.NameView);
                    //표준화
                    this.radGridView1.Rows.Add(commonSystemDynamic.CommonSystemDynamicID, commonSystemDynamic.NameView);
                }

                if (commonSystemDynamics.Count > 0 && this._originIndex.Equals(0))
                {
                    this._originIndex = commonSystemDynamics.Max(m => m.CommonSystemDynamicID);
                    this._lastIndex = this._originIndex + 1;
                }
                if (p_entityPowerEquipmentServices != null)
                {
                    this._entityPowerEquipmentServices = p_entityPowerEquipmentServices;
                }
                else
                {
                    this._entityPowerEquipmentServices = proxy.ReadPowerEquipmentServices(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, 5);
                }

                this.radGridView1.SelectionChanged += new EventHandler(radGridView1_SelectionChanged);

                if (this.radGridView1.Rows.Count > 0)
                {
                    this.radGridView1.CurrentRow = this.radGridView1.Rows[0];
                }
                this.radGridView1.SelectionChanging += radGridView1_SelectionChanging;
            }
        }
        private void MethodSettingRadGridView()
        {
            this.radGridView1.Rows.Clear();
            this.radGridView1.Columns.Clear();

            this.radGridView1.GridViewElement.TableElement.TableHeaderHeight = 25;
            this.radGridView2.GridViewElement.TableElement.TableHeaderHeight = 25;

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                //this.radDropDownListProject.ValueMember = "ProjectID";
                //this.radDropDownListProject.DisplayMember = "ComplexNameView";
                //this.radDropDownListProject.DataSource = proxy.ReadProjects(false).Where(w => w.CaseMainFlag == true).ToList();
                //this.radDropDownListProject.SelectedValue = this._entityEDBInformation.Project.CaseProjectID;

                this._oldCommonSystemDynamics = proxy.ReadCommonSystemDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);

                // radGridView1 Setting

                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "CommonSystemDynamicID",
                    IsVisible = false,
                    ReadOnly = true,
                });

                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "NameView",
                    HeaderText = "System",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    MaxLength = 25,
                    ReadOnly = true
                });

                // radGridView2 Setting
                this.radGridView2.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "PowerEquipmentServiceID",
                    IsVisible = false,
                });
                this.radGridView2.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "Name",
                    HeaderText = "Key",
                    TextAlignment = ContentAlignment.MiddleCenter,
                    IsVisible = false,

                });
                this.radGridView2.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "NameView",
                    HeaderText = "Property",
                    TextAlignment = ContentAlignment.MiddleCenter,
                });

                this.MethodBindingRadGridView();

                this.radDropDownListProject.SelectedIndexChanged += (ss, ee) =>
                {
                    this.MethodBindingRadGridView();
                };
            }
        }
        private void EventMethodCellValidatingRadGridView1(object sender, CellValidatingEventArgs e)
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
                this.MethodSavePreviousPowerEquipmentService();

                var commonSystemDynamicID = Convert.ToInt32(this.radGridView1.SelectedRows[0].Cells["CommonSystemDynamicID"].Value);
                this.radGridView2.Rows.Clear();

                var powerEquipmentServices = this._entityPowerEquipmentServices.Where(w => w.CommonSystemDynamicID.Equals(commonSystemDynamicID)).OrderBy(ob => ob.Orders);
                foreach (var powerEquipmentService in powerEquipmentServices)
                {
                    this.radGridView2.Rows.Add(powerEquipmentService.PowerEquipmentServiceID, powerEquipmentService.Name, powerEquipmentService.NameView);
                }
                if (this.radGridView2.Rows.Count > 0)
                {
                    this.radGridView2.CurrentRow = this.radGridView2.Rows[0];
                }

                //Changing 에서 왔음.
                if (this.radGridView1.SelectedRows[0].Cells["CommonSystemDynamicID"].Value == null ||
                    this.radGridView1.SelectedRows[0].Cells["CommonSystemDynamicID"].Value.Equals(0))
                {
                    this.radGridView1.SelectedRows[0].Cells["CommonSystemDynamicID"].Value = this._lastIndex;
                    this._prevSelectedID = this._lastIndex;
                    this._lastIndex++;
                }
                else
                {
                    this._prevSelectedID = Convert.ToInt32(this.radGridView1.SelectedRows[0].Cells["CommonSystemDynamicID"].Value);
                }

            }
        }
        private void radGridView1_SelectionChanging(object sender, GridViewSelectionCancelEventArgs e)
        {
            if (this.radGridView1.SelectedRows.Count > 0)
            {
                SharedMethod.MethodCreateName(this.radGridView2, "Name", "NameView");
                if (!this.radGridView2.Rows.Select(w => w.Cells["Name"].Value).Distinct().Count().Equals(this.radGridView2.Rows.Count)
                    || !this.radGridView2.Rows.Select(w => w.Cells["NameView"].Value).Distinct().Count().Equals(this.radGridView2.Rows.Count)
                    //|| ( this._entityFrmEquipmentTypeDynamic.ListType.Name.Equals(Enum.GetName(typeof(EDCS.WF.BSL.Enums.EnumNameCheck), EDCS.WF.BSL.Enums.EnumNameCheck.PowerEquipmentList)) ? 
                    //false : !this.radGridView4.Rows.Select(w => w.Cells["NameView"].Value).Distinct().Count().Equals(this.radGridView4.Rows.Count))
                    )
                {
                    e.Cancel = true;
                    //new EDCS.WF.BSL.Helpers.CustomRadMessageBoxShowAndTheme(EDCS.WF.Properties.Resources.Message10044, this._entityFrmEquipmentTypeDynamic.CommonThemeName).RadMessageBoxShow();
                    MessageBox.Show("It is duplicated.");
                    return;
                }

                //Changed 마지막부분으로 옮김.
                //if (this.radGridView1.Rows[this.radGridView1.SelectedRows[0].Index].Cells["EquipmentTypeDynamicID"].Value == null ||
                //    this.radGridView1.Rows[this.radGridView1.SelectedRows[0].Index].Cells["EquipmentTypeDynamicID"].Value.Equals(0))
                //{
                //    this.radGridView1.Rows[this.radGridView1.SelectedRows[0].Index].Cells["EquipmentTypeDynamicID"].Value = this._lastIndex;
                //    this._prevSelectedID = this._lastIndex;
                //    this._lastIndex++;
                //}
                //else
                //{
                //    this._prevSelectedID = Convert.ToInt32( this.radGridView1.Rows[this.radGridView1.SelectedRows[0].Index].Cells["EquipmentTypeDynamicID"].Value);
                //}
            }
        }
        private void EventMethodSelectionChangedRadGridView1(object sender, EventArgs e)
        {
            if (this.radGridView1.SelectedRows.Count > 0)
            {
                this.MethodSavePreviousPowerEquipmentService();

                var commonSystemDynamicID = Convert.ToInt32(this.radGridView1.SelectedRows[0].Cells["CommonSystemDynamicID"].Value);
                this.radGridView2.Rows.Clear();

                var powerEquipmentServices = this._entityPowerEquipmentServices.Where(w => w.CommonSystemDynamicID.Equals(commonSystemDynamicID)).OrderBy(ob => ob.Orders);
                foreach (var powerEquipmentService in powerEquipmentServices)
                {
                    this.radGridView2.Rows.Add(powerEquipmentService.PowerEquipmentServiceID, powerEquipmentService.Name, powerEquipmentService.NameView);
                }
                if (this.radGridView2.Rows.Count > 0)
                {
                    this.radGridView2.CurrentRow = this.radGridView2.Rows[0];
                }

                //Changing 에서 왔음.
                if (this.radGridView1.SelectedRows[0].Cells["CommonSystemDynamicID"].Value == null ||
                    this.radGridView1.SelectedRows[0].Cells["CommonSystemDynamicID"].Value.Equals(0))
                {
                    this.radGridView1.SelectedRows[0].Cells["CommonSystemDynamicID"].Value = this._lastIndex;
                    this._prevSelectedID = this._lastIndex;
                    this._lastIndex++;
                }
                else
                {
                    this._prevSelectedID = Convert.ToInt32(this.radGridView1.SelectedRows[0].Cells["CommonSystemDynamicID"].Value);
                }

            }
        }

    }
}
