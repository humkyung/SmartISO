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

namespace Configuration
{
    public partial class UserControlEDBDistrictType : UserControlBase
    {
        private EntityComplexTreeViewLoadingComplexTableComplexValue _selectedNodeValue;
        private int _originDistrictTypeID;
        private int _lastDistrictTypeID;
        private List<EntityEDB_PowerEquipmentSystemMapping> _powerEquipmentSystemMappings;
        EntityEDB_ComplexTreeViewLoading _oldDistrictTypes;
        private bool _flagLoading;

        public UserControlEDBDistrictType()
        {
            try
            {
                this.InitializeComponent();

                RegisterEventHandler();
                this.MethodDesignForm();

                this.Dock = DockStyle.Fill;
            }
            catch (Exception exception)
            {
                new ErrorHandling(this, exception, this._entityEDBInformation.Employee, this._entityEDBInformation.Project, this._entityEDBInformation.ListType).ErrorMsgDatabaseSave();
            }
        }

        public void RegisterEventHandler()
        {
            // RadDropDownList
            //this.radDropDownListProject.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListProject_SelectedIndexChanged);
            //this.radDropDownListListType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListListType_SelectedIndexChanged);

            // RadGridView
            this.radGridViewDistrictType.RowsChanged += new GridViewCollectionChangedEventHandler(radGridViewDistrictType_RowsChanged);
            this.radGridViewDistrictType.RowsChanging += new GridViewCollectionChangingEventHandler(radGridViewDistrictType_RowsChanging);
            this.radGridViewDistrictType.CellValidating += new CellValidatingEventHandler(radGridViewDistrictType_CellValidating);
            this.radGridViewDistrictType.ValueChanged += new EventHandler(radGridViewDistrictType_ValueChanged);

            // RadTreeView
            this.radTreeViewPaint.NodeExpandedChanging += new RadTreeView.RadTreeViewCancelEventHandler(radTreeViewPaint_NodeExpandedChanging);
            this.radTreeViewPaint.ContextMenuOpening += new TreeViewContextMenuOpeningEventHandler(radTreeViewPaint_ContextMenuOpening);
            this.radTreeViewPaint.PreviewKeyDown += new PreviewKeyDownEventHandler(radTreeViewPaint_PreviewKeyDown);

            this.radTreeViewPaint.SelectedNodeChanged += new RadTreeView.RadTreeViewEventHandler(radTreeViewPaint_SelectedNodeChanged);
            //this.radTreeViewPaint.AllowDrop= true;

            this.radTreeViewPaint.DragEnded += new RadTreeView.DragEndedHandler(radTreeViewPaint_DragEnded);
            this.radTreeViewPaint.DragEnding += new RadTreeView.DragEndingHandler(radTreeViewPaint_DragEnding);
            // RadContextMenu
            this.radContextMenu1RadMenuItemAdd.Click += new EventHandler(radContextMenu1RadMenuItemAdd_Click);
            this.radContextMenu1RadMenuItemDelete.Click += new EventHandler(radContextMenu1RadMenuItemDelete_Click);
            this.radContextMenu1RadMenuItemUpdate.Click += new EventHandler(radContextMenu1RadMenuItemUpdate_Click);
        }
        private void MethodDesignForm()
        {
            this.radTreeViewPaint.AllowDragDrop = true;

            // radDropDownListProjectNo
            this.radDropDownListProject.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
            this.radDropDownListProject.DefaultItemsCountInDropDown = 10;
            this.radDropDownListProject.MaxDropDownItems = 10;
            this.radDropDownListProject.EnableMouseWheel = false;
            this.radDropDownListProject.Cursor = Cursors.Hand;
            this.radDropDownListProject.Font = new Font("Tahoma", 8.25F, FontStyle.Bold);
            this.radDropDownListProject.ListElement.Font = new Font("Tahoma", 8.25F);

            // radDropDownListListType
            this.radDropDownListListType.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
            this.radDropDownListListType.DefaultItemsCountInDropDown = 10;
            this.radDropDownListListType.MaxDropDownItems = 10;
            this.radDropDownListListType.EnableMouseWheel = false;
            this.radDropDownListListType.Cursor = Cursors.Hand;
            this.radDropDownListListType.Font = new Font("Tahoma", 8.25F, FontStyle.Bold);
            this.radDropDownListListType.ListElement.Font = new Font("Tahoma", 8.25F);

            // radGroupBoxDistrictType
            // radGridViewDistrictType
            this.radGridViewDistrictType.AllowAutoSizeColumns = false;
            this.radGridViewDistrictType.AllowColumnChooser = false;
            this.radGridViewDistrictType.AllowColumnHeaderContextMenu = false;
            this.radGridViewDistrictType.AllowColumnReorder = false;
            this.radGridViewDistrictType.AllowMultiColumnSorting = false;
            this.radGridViewDistrictType.AllowRowResize = false;
            this.radGridViewDistrictType.AutoGenerateColumns = false;
            this.radGridViewDistrictType.AutoSizeColumnsMode = GridViewAutoSizeColumnsMode.Fill;
            this.radGridViewDistrictType.EnableHotTracking = false;
            this.radGridViewDistrictType.ShowCellErrors = false;
            this.radGridViewDistrictType.ShowFilteringRow = false;
            this.radGridViewDistrictType.ShowNoDataText = false;
            this.radGridViewDistrictType.ShowRowErrors = false;
            this.radGridViewDistrictType.TableElement.TableHeaderHeight = 40;
            this.radGridViewDistrictType.TableElement.RowHeight = 30;
            //
            this.radGridViewDistrictType.AllowColumnResize = false;
            this.radGridViewDistrictType.AllowDragToGroup = false;
            this.radGridViewDistrictType.EnableGrouping = false;
            this.radGridViewDistrictType.EnableSorting = false;
            this.radGridViewDistrictType.ShowColumnHeaders = false;
            this.radGridViewDistrictType.ShowGroupPanel = false;
            this.radGridViewDistrictType.AllowCellContextMenu = false;
            this.radGridViewDistrictType.AllowAddNewRow = false;

            this.radGridViewDistrictType.ShowRowHeaderColumn = false;
            //
            this.radGridViewDistrictType.AllowRowReorder = false;
            //this.radGridViewDistrictType.AllowAddNewRow = false;
            this.radGridViewDistrictType.ShowRowHeaderColumn = false;
            //this.radGridViewDistrictType.AllowCellContextMenu = false;
            //this.radGridViewDistrictType.Allow= false;

            // radGroupBoxPaint

            // radTreeViewPaint
            this.radTreeViewPaint.AutoCheckChildNodes = false;
            this.radTreeViewPaint.Cursor = Cursors.Hand;
            this.radTreeViewPaint.HideSelection = false;
            this.radTreeViewPaint.HotTracking = false;
            this.radTreeViewPaint.ItemHeight = 22;
            this.radTreeViewPaint.LazyMode = false;
            this.radTreeViewPaint.LineStyle = TreeLineStyle.Solid;
            this.radTreeViewPaint.ShowDragHint = true;
            this.radTreeViewPaint.ShowDropHint = true;
            this.radTreeViewPaint.ShowLines = true;

            this.radTreeViewPaint.RadContextMenu = this.radContextMenu1;

            //radContextMenu1

            //radContextMenu1RadMenuItemAdd
            this.radContextMenu1RadMenuItemAdd.Text = "  Add";
            this.radContextMenu1RadMenuItemAdd.Margin = new Padding(1, 1, 1, 1);

            //radContextMenu1RadMenuItemUpdate
            this.radContextMenu1RadMenuItemUpdate.Text = "  Update";
            this.radContextMenu1RadMenuItemUpdate.Margin = new Padding(1, 1, 1, 1);

            //radContextMenu1RadMenuItemDelete
            this.radContextMenu1RadMenuItemDelete.Text = "  Delete";
            this.radContextMenu1RadMenuItemDelete.Margin = new Padding(1, 1, 1, 1);

            this.radGridViewDistrictType.AllowDeleteRow = false;
        }

        private void MethodBindingPBSMode()
        {
            //if (!this._entityFrmDistrictType.enumPBS.Equals(EnumPBS.None))
            //{
            //    var node = FindNodeRecursive(this.radTreeViewPaint.Nodes, this._entityFrmDistrictType.districtDynamicID);
            //    if (node != null)
            //    {
            //        node.Selected = true;
            //        this._selectedNodeValue = (EntityComplexTreeViewLoadingComplexTableComplexValue)node.Tag;

            //        switch (this._entityFrmDistrictType.enumPBS)
            //        {
            //            case EnumPBS.Update:
            //                this.EventMethodClickRadContextMenu1RadMenuItemUpdate(null, null);
            //                break;
            //            case EnumPBS.Add:
            //                this.EventMethodClickRadContextMenu1RadMenuItemAdd(null, null);
            //                break;
            //            case EnumPBS.Delete:
            //                this.EventMethodClickRadContextMenu1RadMenuItemDelete(null, null);
            //                break;
            //            default:
            //                break;
            //        }
            //    }
            //}
        }
        private void MethodBindingRadDropDownListListType()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var listTypes = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                if (listTypes.Count > 0)
                {
                    this.radDropDownListListType.Items.Clear();
                    this.radDropDownListListType.ValueMember = "ListTypeID";
                    this.radDropDownListListType.DisplayMember = "ComplexNameView";
                    this.radDropDownListListType.DataSource = listTypes;
                    //this.radDropDownListListType.SelectedValue = null;
                    this.radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;
                    this.radDropDownListListType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListListType_SelectedIndexChanged);
                    this.radDropDownListListType.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListListType_SelectedIndexChanging);

                }
                else
                {
                    //throw new Exception(Resources.Message20003);
                }
            }
        }
        private void MethodBindingRadDropDownListProjectNo()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var projects = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true).OrderByDescending(odb => odb.No).ToList();
                if (projects.Count > 0)
                {
                    projects.ConvertAll(ca => ca.ComplexNameView + " - " + ca.Type);
                    this.radDropDownListProject.Items.Clear();
                    this.radDropDownListProject.ValueMember = "ProjectID";
                    this.radDropDownListProject.DisplayMember = "ComplexNameView";
                    this.radDropDownListProject.DataSource = projects;
                    //this.radDropDownListProject.SelectedValue = null;
                    this.radDropDownListProject.SelectedValue = this._entityEDBInformation.Project.ProjectID;
                    this.radDropDownListProject.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListProject_SelectedIndexChanged);
                    this.radDropDownListProject.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListProject_SelectedIndexChanging);
                }
                else
                {
                    //throw new Exception(Resources.Message20002);
                }
            }
        }
        private void MethodBindingRadTreeView()
        {
            var dataTable = new DataTable("Table1");

            // Columns Setting
            dataTable.Columns.Add("ID", typeof(int));
            dataTable.Columns.Add("ParentID", typeof(int));
            dataTable.Columns.Add("DistrictTypeNameView", typeof(string));

            // Rows Setting
            dataTable.Rows.Add(1, default(int), this.radDropDownListListType.Text);
            foreach (var row in this.radGridViewDistrictType.Rows)
            {
                dataTable.Rows.Add(row.Index + 2, row.Index + 1, row.Cells["NameView"].Value);
            } // end foreach


            this.radTreeViewPaint.ValueMember = "ID";
            this.radTreeViewPaint.DisplayMember = "DistrictTypeNameView";
            this.radTreeViewPaint.ChildMember = "ID";
            this.radTreeViewPaint.ParentMember = "ParentID";
            this.radTreeViewPaint.DataSource = dataTable;

            this.radTreeViewPaint.Nodes[0].Font = new Font("Tahoma", 9F, FontStyle.Bold); // RootNode Change
            this.radTreeViewPaint.ExpandAll(); // Node 전체 확장
        }

        public override void SaveData()
        {
            this.MethodSaveDistrictType(true, this._entityEDBInformation.ListType.ListTypeID);
            #region ProcessEquipmentList 일때 HazardousSourceList 에 PBS 복사 :: 단, 초기 한번 만!!!
            if (this._entityEDBInformation.ListType.ListTypeID == (int)EnumNameCheck.ProcessEquipmentList)
            {
                try
                {
                    var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                    using (proxy as IDisposable)
                    {
                        
                        List<EntityEDB_DistrictType> simpleDistrictTypes = null;
                        simpleDistrictTypes = new List<EntityEDB_DistrictType>(proxy.ReadSimpleDistrictTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.ProjectID, (int)EnumNameCheck.HazardousSourceList));
                        if (simpleDistrictTypes.Count == 0 && this._entityEDBInformation.Project.ListTypeFlag.Contains((int)EnumNameCheck.HazardousSourceList))
                        {
                            this.MethodSaveDistrictType(true, (int)EnumNameCheck.HazardousSourceList);
                        }
                        simpleDistrictTypes = new List<EntityEDB_DistrictType>(proxy.ReadSimpleDistrictTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.ProjectID, (int)EnumNameCheck.BuildingList));
                        if (simpleDistrictTypes.Count == 0 && this._entityEDBInformation.Project.ListTypeFlag.Contains((int)EnumNameCheck.BuildingList))
                        {
                            this.MethodSaveDistrictType(true, (int)EnumNameCheck.BuildingList);
                        }
                        simpleDistrictTypes = new List<EntityEDB_DistrictType>(proxy.ReadSimpleDistrictTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.ProjectID, (int)EnumNameCheck.StructuralEquipmentList));
                        if (simpleDistrictTypes.Count == 0 && this._entityEDBInformation.Project.ListTypeFlag.Contains((int)EnumNameCheck.StructuralEquipmentList))
                        {
                            this.MethodSaveDistrictType(true, (int)EnumNameCheck.StructuralEquipmentList);
                        }
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
            #endregion
        }
        public override void LoadData()
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    this.MethodBindingRadDropDownListListType();
                    this.MethodBindingRadDropDownListProjectNo();

                    this._powerEquipmentSystemMappings = proxy.ReadPowerEquipmentSystemMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.CaseProjectID);

                    this.MethodSettingRadGridViewDistrictType();
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

        private void MethodSaveDistrictDynamic(ref List<EDB_DistrictType> p_districtTypes, RadTreeNodeCollection p_nodes)
        {
            foreach (var node in p_nodes)
            {
                if (node.Nodes.Count > 0)
                {
                    this.MethodSaveDistrictDynamic(ref p_districtTypes, node.Nodes);
                }

                var maxDistrictType = this.radGridViewDistrictType.Rows.Count * 100;
                var tag = node.Tag as EntityComplexTreeViewLoadingComplexTableComplexValue;
                if (tag != null)
                {
                    if (!tag.NodeTypeName.Equals("LISTTYPE"))
                    {
                        //DH 신규프로젝트의 경우 p_districType 에 ID 가 존재하지 않는다 그러므로 orders 로 찾음.
                        //var districtType = p_districtTypes.Where(w => w.DistrictTypeID.Equals(tag.DistrictTypeID)).SingleOrDefault();
                        int orders = node.Level * 100;
                        var districtType = p_districtTypes.Where(w => w.Orders.Equals(orders)).SingleOrDefault();
                        if (districtType != null)
                        {
                            var districtDynamic = new EDB_DistrictDynamic();
                            districtDynamic.DistrictTypeID = tag.DistrictTypeID;
                            districtDynamic.DistrictDynamicID = tag.DistrictDynamicID;
                            districtDynamic.ParentID = tag.DistrictDynamicParentID;
                            districtDynamic.Orders = tag.DistrictDynamicOrders;
                            districtDynamic.Name = tag.DistrictDynamicName;
                            districtDynamic.NameView = tag.DistrictDynamicNameView;

                            //if (tag.DistrictTypeOrders.Equals(maxDistrictType) && (tag.DistrictDynamicID.Equals(0) || tag.DistrictTypeID > this._lastDistrictTypeID))
                            if (tag.DistrictTypeOrders.Equals(maxDistrictType))
                            {
                                if (tag.SheetDynamicID.Equals(0))
                                {
                                    districtDynamic.EDB_SheetDynamic.Add(new EDB_SheetDynamic()
                                    {
                                        SheetDynamicID = tag.SheetDynamicID,
                                        DistrictDynamicID = tag.DistrictDynamicID,
                                        Orders = 100,
                                        Name = tag.DistrictDynamicName,
                                        NameView = tag.DistrictDynamicNameView,
                                        TotalRowCount = tag.TotalRowCount
                                    });
                                }
                                else
                                {
                                    districtDynamic.EDB_SheetDynamic.Add(new EDB_SheetDynamic()
                                    {
                                        TotalRowCount = tag.TotalRowCount
                                    });
                                }
                            }
                            districtType.EDB_DistrictDynamic.Add(districtDynamic);
                        }
                    }
                }
            }
        }
        private void MethodSaveDistrictTypeCase(List<EDB_DistrictType> p_districtTypes, int p_listTypeID)
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                /////Name, NameView, Orders, DeleteFlag
                var caseProjects = proxy.ReadCaseProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.CaseProjectID, p_listTypeID).Where(w => w.CaseMainFlag == false).ToList();

                var caseDistrictTypes = proxy.ReadCaseSimpleDistrictTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.CaseProjectID, p_listTypeID);
                caseDistrictTypes.RemoveAll(ra => ra.ProjectID == this._entityEDBInformation.Project.CaseProjectID);
                var caseDistrictDynamics = proxy.ReadCaseDistrictDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.CaseProjectID, p_listTypeID);
                caseDistrictDynamics.RemoveAll(ra => ra.ProjectID == this._entityEDBInformation.Project.CaseProjectID);
                var caseSheetDynamics = proxy.ReadSheetDynamicsCaseProject(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.CaseProjectID, p_listTypeID);
                caseSheetDynamics.RemoveAll(ra => ra.ProjectID == this._entityEDBInformation.Project.CaseProjectID);

                if (caseDistrictTypes.Count == 0
                    && caseDistrictDynamics.Count == 0
                    && caseSheetDynamics.Count == 0)
                {
                    return;
                }
                var updateDistrictTypes = new List<EntityEDB_DistrictType>();
                var updateDistrictDynamics = new List<EntityEDB_DistrictDynamic>();
                var updateSheetDynamics = new List<EntityEDB_SheetDynamic>();
                try
                {
                    var districtDynamicAll = new List<EDB_DistrictDynamic>();
                    foreach (var districtType in p_districtTypes)
                    {
                        var oldDitrictType = this._oldDistrictTypes.DistrictTypes.SingleOrDefault(w => w.DistrictTypeID == districtType.DistrictTypeID);
                        if (oldDitrictType != null)
                        {
                            if (oldDitrictType.Name != districtType.Name || oldDitrictType.NameView != districtType.NameView)
                            {
                                var caseDistrictType = caseDistrictTypes.Where(w => w.MasterDistrictTypeID == districtType.DistrictTypeID).ToList();
                                caseDistrictType.ConvertAll(ca => ca.Name = districtType.Name);
                                caseDistrictType.ConvertAll(ca => ca.NameView = districtType.NameView);
                                updateDistrictTypes.AddRange(caseDistrictType);
                            }
                        }

                        districtDynamicAll.AddRange(districtType.EDB_DistrictDynamic);
                        foreach (var districtDynamic in districtType.EDB_DistrictDynamic)
                        {
                            var caseDistrictDynamic = caseDistrictDynamics.Where(w => w.MasterDistrictDynamicID == districtDynamic.DistrictDynamicID).ToList();
                            caseDistrictDynamic.ConvertAll(ca => ca.Orders = districtDynamic.Orders);
                            //수정된거 찾음
                            var oldDistrictDynamic = this._oldDistrictTypes.DistrictDynamics.SingleOrDefault(w => w.ComplexValue.DistrictDynamicID == districtDynamic.DistrictDynamicID);
                            if (oldDistrictDynamic != null)
                            {
                                if (oldDistrictDynamic.ComplexValue.DistrictDynamicNameView != districtDynamic.NameView)
                                {
                                    caseDistrictDynamic.ConvertAll(ca => ca.NameView = districtDynamic.NameView);
                                    foreach (var casedistrict in caseDistrictDynamic)
                                    {
                                        var caseSheetDynamic = caseSheetDynamics.Where(w => w.DistrictDynamicID == casedistrict.DistrictDynamicID).ToList();
                                        caseSheetDynamic.ConvertAll(ca => ca.NameView = districtDynamic.NameView);
                                        updateSheetDynamics.AddRange(caseSheetDynamic);
                                    }
                                    updateDistrictDynamics.AddRange(caseDistrictDynamic);
                                }
                            }
                        }
                    }
                    proxy.UpdateComplexTreeView(EDCS.WF.BSL.Variable.SharedVariable.IsIE, updateDistrictTypes, updateDistrictDynamics, updateSheetDynamics);

                    ///
                    //Orders다시 부여.
                    foreach (var districtDynamicsProject in caseDistrictDynamics.GroupBy(gb => gb.ProjectID))
                    {
                        var newOrders = 100;
                        foreach (var district in districtDynamicsProject.OrderBy(ob => ob.Orders))
                        {
                            district.Orders = newOrders;
                            newOrders += 100;
                        }
                    }
                    //DistrictDynamic 삭제 된거 찾아야됨
                    foreach (var districtDynamic in caseDistrictDynamics.GroupBy(gb => gb.MasterDistrictDynamicID))
                    {
                        //삭제 찾기
                        if (districtDynamicAll.Where(w => w.DistrictDynamicID == districtDynamic.Key).Count() == 0)
                        {
                            //삭제됨.
                            districtDynamic.ToList().ConvertAll(ca => ca.DeleteFlag = true);
                            foreach (var projectDistrictDynamics in districtDynamic.GroupBy(gb => gb.ProjectID))
                            {
                                caseDistrictDynamics.Where(w => w.ProjectID == projectDistrictDynamics.Key && w.Orders > projectDistrictDynamics.ElementAt(0).Orders).ToList().ConvertAll(ca => ca.Orders = ca.Orders - 100);
                            }
                            caseSheetDynamics.Where(w => w.MasterDistrictDynamicID == districtDynamic.Key).ToList().ConvertAll(ca => ca.DeleteFlag = true);
                        }
                    }
                    proxy.UpdateDistrictDynamicOrdersAndDelete(EDCS.WF.BSL.Variable.SharedVariable.IsIE, caseDistrictDynamics, caseSheetDynamics);
                }
                catch (ObjectDisposedException e)
                {

                }
                ///

                //foreach (var districtTypeGroup in caseDistrictTypes.GroupBy(gb => gb.ProjectID))
                //{
                //foreach (var project in caseProjects)
                //{
                //        var caseDistrictType = caseDistrictTypes.Where(w => w.MasterDistrictTypeID == districtType.DistrictTypeID && w.ProjectID == project.ProjectID);
                //        updateDistrictTypes .AddRange(this.MethodCompareDistrictType(districtType, caseDistrictType.ToList()));
                //            var caseDistrictDynamic = caseDistrictDynamics.Where(w => w.MasterDistrictDynamicID == districtDynamic.DistrictDynamicID && w.ProjectID == project.ProjectID);
                //            updateDistrictDynamics .AddRange(this.MethodCompareDistrictDynamic(districtDynamic, caseDistrictDynamic.ToList()));
                //                var caseSheetDynamic = caseSheetDynamics.Where(w => w.MasterSheetDynamicID == sheetDynamic.SheetDynamicID && w.ProjectID == project.ProjectID);
                //                updateSheetDynamics.AddRange(this.MethodCompareSheetDynamic(sheetDynamic, caseSheetDynamic.ToList()));
                //}
                //}
                ///
            }
        }
        private void MethodBindingSystemMapping()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var PowerEquipmentSystemMappings = proxy.ReadPowerEquipmentSystemMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.CaseProjectID);

                foreach (var item in PowerEquipmentSystemMappings)
                {
                    _powerEquipmentSystemMappings.Add(new EntityEDB_PowerEquipmentSystemMapping()
                    {
                        PowerEquipmentSystemMappingID = item.PowerEquipmentSystemMappingID,
                        CommonSystemDynamicID = item.CommonSystemDynamicID,
                        ProjectID = item.ProjectID,
                        Orders = item.Orders,
                        Name = item.Name,
                        NameView = item.NameView,
                        ModifiedDate = item.ModifiedDate,
                    });
                }
            }
        }
        private void MethodDeleteDistrictType(int p_districtTypeID, RadTreeNodeCollection p_nodes)
        {
            int count = p_nodes.Count;
            for (int i = 0; i < count; i++)
            {
                if (p_nodes[i].Nodes.Count > 0)
                {
                    this.MethodDeleteDistrictType(p_districtTypeID, p_nodes[i].Nodes);
                }
                if (((EntityComplexTreeViewLoadingComplexTableComplexValue)p_nodes[i].Tag).DistrictTypeID.Equals(p_districtTypeID))
                {
                    p_nodes[i].Remove();
                    count--;
                    i--;
                }
            }
        }
        private void MethodSaveDistrictType(bool p_flagDialogResult, int p_listTypeID)
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                if (this.radGridViewDistrictType.RowCount > 0)
                {
                    bool flagSaveWhether = true;
                    for (int i = 0; i < this.radGridViewDistrictType.RowCount; i++)
                    {
                        for (int j = 2; j < this.radGridViewDistrictType.ColumnCount; j++) // j가 1부터 시작되는 이유 : 첫번째 Column 제외 (ID값은 추가시 자동으로 부여되기 때문에 없어야 함), 두번째 Column 제외 (Name이 없으면 NameView로 저장)
                        {
                            if (this.radGridViewDistrictType.Rows[i].Cells[j].Value == null && !(this.radGridViewDistrictType.Rows[i].Cells[j].ColumnInfo is GridViewCheckBoxColumn))
                            {
                                flagSaveWhether = false;
                                break;
                            }
                        }
                    }

                    if (flagSaveWhether)
                    {
                        this._oldDistrictTypes = proxy.ReadComplexTreeViewLoading(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.CaseProjectID, p_listTypeID);
                        this._oldDistrictTypes.ListTypes.ConvertAll(ca => ca.ComplexNameView = this._entityEDBInformation.Project.NameView);
                        var districtTypes = new List<EDB_DistrictType>();
                        //객체로 생성.
                        foreach (var row in this.radGridViewDistrictType.Rows.OrderBy(ob => ob.Index))
                        {
                            districtTypes.Add(new EDB_DistrictType()
                            {
                                DistrictTypeID = Convert.ToInt32(row.Cells["DistrictTypeID"].Value),
                                ListTypeID = p_listTypeID,
                                ProjectID = this._entityEDBInformation.Project.ProjectID,
                                Orders = (row.Index + 1) * 100,
                                Name = row.Cells["Name"].Value == null ? row.Cells["NameView"].Value.ToString().Trim() : row.Cells["Name"].Value.ToString().Trim(),
                                NameView = row.Cells["NameView"].Value.ToString().Trim(),
                                RevisionUnit = this.radGridViewDistrictType.Rows.Count.Equals(row.Index + 1) ? true : false,
                            });
                        }
                        if (this._entityEDBInformation.ListType.Name.Equals(Enum.GetName(typeof(EnumNameCheck), EnumNameCheck.PowerEquipmentList)))
                        {
                            //발전은  리비전을 플랜트 단위로...
                            districtTypes.ForEach(fe => fe.RevisionUnit = false);
                        }
                        this.MethodSaveDistrictDynamic(ref districtTypes, this.radTreeViewPaint.Nodes);

                        foreach (var districtType in districtTypes)
                        {
                            if (districtType.DistrictTypeID > this._lastDistrictTypeID)
                            {
                                districtType.DistrictTypeID = 0;
                            }
                        } // end foreach

                        proxy.CreateDistrictType(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref districtTypes, this._entityEDBInformation.Project.CaseProjectID, p_listTypeID);
                        proxy.UpdateDistrictTypeMasterID(EDCS.WF.BSL.Variable.SharedVariable.IsIE, districtTypes);


                        ///모든 Case 에 적용.
                        this.MethodSaveDistrictTypeCase(districtTypes, p_listTypeID);


                        if (this._entityEDBInformation.ListType.Name.Equals(Enum.GetName(typeof(EnumNameCheck), EnumNameCheck.PowerEquipmentList)))
                        {
                            // System Mapping Save
                            var powerEquipmentSystemMappings = new List<EDB_PowerEquipmentSystemMapping>();
                            foreach (var item in this._powerEquipmentSystemMappings)
                            {
                                var powerEquipmentSystemMapping = new EDB_PowerEquipmentSystemMapping();
                                powerEquipmentSystemMapping.PowerEquipmentSystemMappingID = item.PowerEquipmentSystemMappingID;
                                powerEquipmentSystemMapping.CommonSystemDynamicID = item.CommonSystemDynamicID;
                                powerEquipmentSystemMapping.ProjectID = item.ProjectID;
                                powerEquipmentSystemMapping.Orders = item.Orders;
                                powerEquipmentSystemMapping.ModifiedDate = item.ModifiedDate;
                                powerEquipmentSystemMapping.Name = item.Name;
                                powerEquipmentSystemMapping.NameView = item.NameView;

                                powerEquipmentSystemMappings.Add(powerEquipmentSystemMapping);
                            }
                            proxy.CreatePowerEquipmentSystemMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, powerEquipmentSystemMappings, this._entityEDBInformation.Project.ProjectID);
                        }
                        //new CustomRadMessageBoxShowAndTheme(Resources.Message10007).RadMessageBoxShow();
                        //if (p_flagDialogResult)
                        //{
                        //this.DialogResult = DialogResult.OK;
                        //}
                        //var districtTypes = new List<EDCS.WF.DSL.DistrictType>();

                        //var sheetDynamics = proxy.ReadSheetDynamics(false, this._project.ProjectID, this._listType.ListTypeID);
                        //foreach (var row in this.radGridViewDistrictType.Rows)
                        //{
                        //    var districtType = new EDCS.WF.DSL.DistrictType();

                        //    if (!this.radGridViewDistrictType.Rows.Count.Equals(row.Index + 1) && !sheetDynamics.Where(w => w.DistrictTypeID.Equals(Convert.ToInt32(row.Cells["DistrictTypeID"].Value))).Count().Equals(0))
                        //    {
                        //        new EDCS.WF.BSL.Helpers.CustomRadMessageBoxShowAndTheme(EDCS.WF.Properties.Resources.Message10038, this._entityFrmDistrictType.CommonThemeName).RadMessageBoxShow();
                        //        return;
                        //    }

                        //    districtType.DistrictTypeID = Convert.ToInt32(row.Cells["DistrictTypeID"].Value);
                        //    districtType.ProjectID = this._project.ProjectID;
                        //    districtType.ListTypeID = this._listType.ListTypeID;
                        //    districtType.Orders = (row.Index + 1) * 100;
                        //    if (row.Cells["Name"].Value != null)
                        //    {
                        //        districtType.Name = row.Cells["Name"].Value.ToString().Trim();
                        //    }
                        //    else
                        //    {
                        //        districtType.Name = row.Cells["NameView"].Value.ToString().Trim();
                        //    }
                        //    districtType.NameView = row.Cells["NameView"].Value.ToString().Trim();
                        //    districtType.RevisionUnit =Convert.ToBoolean( row.Cells["RevisionUnit"].Value);

                        //    districtTypes.Add(districtType);
                        //}

                        //proxy.CreateDistrictType(districtTypes, this._project.ProjectID, this._listType.ListTypeID);

                        //new EDCS.WF.BSL.Helpers.CustomRadMessageBoxShowAndTheme(EDCS.WF.Properties.Resources.Message10007, this._entityFrmDistrictType.CommonThemeName).RadMessageBoxShow();

                        //this._flagForcedClose = true;
                        //if (p_flagDialogResult)
                        //{
                        //    this.DialogResult = DialogResult.OK;
                        //}
                    }
                    else
                    {
                        //new CustomRadMessageBoxShowAndTheme(Resources.Message10012).RadMessageBoxShow();
                    }
                }
                else
                {
                    //new CustomRadMessageBoxShowAndTheme(Resources.Message10006).RadMessageBoxShow();
                }
                // Save Setting Log
                //SharedMethod.MethodSaveSettingLog(this._entityFrmDistrictType.Project.No, this._entityFrmDistrictType.Employee, this.Text.ToString());
            }
        }
        private void MethodSettingRadGridViewDistrictType()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                this._flagLoading = false;
                //Stopwatch stopwatch = new Stopwatch();
                //stopwatch.Start();
                //var districtTypes = proxy.ReadDistrictTypes(false, this._project.ProjectID, this._listType.ListTypeID);
                //콤보박스에 선택된 프로젝트 이므로 Master 프로젝트임 헷가리지 마
                var complexTreeViewLoad = proxy.ReadComplexTreeViewLoading(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.ProjectID, this._entityEDBInformation.ListType.ListTypeID);
                complexTreeViewLoad.ListTypes.ConvertAll(ca => ca.ComplexNameView = this._entityEDBInformation.Project.NameView);
                //stopwatch.Stop();
                //Debug.WriteLine("MethodSettingRadGridViewDistrictType {0}", stopwatch.ElapsedMilliseconds);
                //var complexTables = complexTreeViewLoad.ListTypes.Concat(complexTreeViewLoad.DistrictDynamics);
                var complexTables = complexTreeViewLoad.ListTypes.Concat(complexTreeViewLoad.DistrictDynamics.Concat(complexTreeViewLoad.SheetDynamics));
                this.radTreeViewPaint.Nodes.Clear();
                this.radTreeViewPaint.Nodes.Add(this.MethodBindingRadTreeViewRecursiveFunction(null, -100, complexTables));
                if (complexTreeViewLoad.DistrictTypes.Count > 0)
                {
                    this._lastDistrictTypeID = complexTreeViewLoad.DistrictTypes.Max(m => m.DistrictTypeID);
                }
                else
                {

                }
                this._originDistrictTypeID = this._lastDistrictTypeID + 1;

                this.radGridViewDistrictType.Columns.Clear();
                this.radGridViewDistrictType.Rows.Clear();

                // Columns Setting
                this.radGridViewDistrictType.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "DistrictTypeID",
                    IsVisible = false,
                });
                this.radGridViewDistrictType.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "Name",
                    TextAlignment = ContentAlignment.MiddleCenter,
                    IsVisible = false,
                });
                this.radGridViewDistrictType.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "NameView",
                    TextAlignment = ContentAlignment.MiddleCenter,
                });
                this.radGridViewDistrictType.Columns.Add(new GridViewCheckBoxColumn()
                {
                    Name = "RevisionUnit",
                    TextAlignment = ContentAlignment.MiddleCenter,
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    IsVisible = false,
                    //
                    Width = 20,
                });

                // Rows Setting
                foreach (var districtType in complexTreeViewLoad.DistrictTypes)
                {
                    this.radGridViewDistrictType.Rows.Add(districtType.DistrictTypeID, districtType.Name, districtType.NameView, districtType.RevisionUnit);
                } // end foreach
                if (this.radGridViewDistrictType.Rows.Count < 1)
                {
                    var commonDistrictTypes = proxy.ReadCommonDistrictTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.ListType.ListTypeID);
                    foreach (var districtType in commonDistrictTypes)
                    {
                        this.radGridViewDistrictType.Rows.Add(0, districtType.Name, districtType.NameView, null);
                    }

                    //    this.EventMethodClickRadButtonMoveAll(null, null);
                }
                this._flagLoading = true;
                //this.MethodBindingRadTreeView();
            }
        }
        private void MethodUpdateDistrictType(int p_districtTypeID, string p_value, RadTreeNodeCollection p_nodes)
        {
            int count = p_nodes.Count;
            for (int i = 0; i < count; i++)
            {
                if (p_nodes[i].Nodes.Count > 0)
                {
                    this.MethodUpdateDistrictType(p_districtTypeID, p_value, p_nodes[i].Nodes);
                }
                if (((EntityComplexTreeViewLoadingComplexTableComplexValue)p_nodes[i].Tag).DistrictTypeID.Equals(p_districtTypeID))
                {
                    ((EntityComplexTreeViewLoadingComplexTableComplexValue)p_nodes[i].Tag).DistrictTypeName = p_value;
                    ((EntityComplexTreeViewLoadingComplexTableComplexValue)p_nodes[i].Tag).DistrictTypeNameView = p_value;

                    p_nodes[i].Text = String.Format("{0} ({1})"
                        , ((EntityComplexTreeViewLoadingComplexTableComplexValue)p_nodes[i].Tag).DistrictDynamicNameView
                        , p_value);
                }
            }
        }

        private List<EDB_DistrictType> MethodCompareDistrictType(EDB_DistrictType p_originDistrictType, List<EntityEDB_DistrictType> p_compareDistrictTypes)
        {
            var returnValue = new List<EDB_DistrictType>();
            foreach (var compare in p_compareDistrictTypes)
            {
                if (p_originDistrictType.Orders != compare.Orders
                    || p_originDistrictType.DeleteFlag != compare.DeleteFlag
                    || p_originDistrictType.Name != compare.Name
                    || p_originDistrictType.NameView != compare.NameView)
                {
                    returnValue.Add(new EDB_DistrictType()
                    {
                        DistrictTypeID = compare.DistrictTypeID,
                        DeleteFlag = compare.DeleteFlag,
                        Orders = compare.Orders,
                        Name = compare.Name,
                        NameView = compare.NameView
                    });
                }
            }
            return returnValue;
        }
        private List<EDB_DistrictDynamic> MethodCompareDistrictDynamic(EDB_DistrictDynamic p_originDistrictDynamic, List<EntityEDB_DistrictDynamic> p_compareDistrictDynamics)
        {
            var returnValue = new List<EDB_DistrictDynamic>();
            foreach (var compare in p_compareDistrictDynamics)
            {
                if (p_originDistrictDynamic.Orders != compare.Orders
                    || p_originDistrictDynamic.DeleteFlag != compare.DeleteFlag
                    || p_originDistrictDynamic.Name != compare.Name
                    || p_originDistrictDynamic.NameView != compare.NameView)
                {
                    returnValue.Add(new EDB_DistrictDynamic()
                    {
                        DistrictDynamicID = compare.DistrictDynamicID,
                        DeleteFlag = compare.DeleteFlag,
                        Orders = compare.Orders,
                        Name = compare.Name,
                        NameView = compare.NameView
                    });
                }
            }
            return returnValue;
        }
        private List<EDB_SheetDynamic> MethodCompareSheetDynamic(EDB_SheetDynamic p_originSheetDynamic, List<EntityEDB_SheetDynamic> p_compareSheetDynamics)
        {
            var returnValue = new List<EDB_SheetDynamic>();
            foreach (var compare in p_compareSheetDynamics)
            {
                if (p_originSheetDynamic.DeleteFlag != compare.DeleteFlag
                    || p_originSheetDynamic.NameView != compare.NameView)
                {
                    returnValue.Add(new EDB_SheetDynamic()
                    {
                        SheetDynamicID = compare.SheetDynamicID,
                        DeleteFlag = compare.DeleteFlag,
                        NameView = compare.NameView
                    });
                }
            }
            return returnValue;
        }
        private RadTreeNode FindNodeRecursive(RadTreeNodeCollection nodes, int p_districtDynamicID)
        {
            foreach (RadTreeNode node in nodes)
            {
                if (((EntityComplexTreeViewLoadingComplexTableComplexValue)node.Tag).DistrictDynamicID == p_districtDynamicID)
                {
                    return node;
                }

                if (node.Nodes.Count > 0)
                {
                    var childNode = FindNodeRecursive(node.Nodes, p_districtDynamicID);
                    if (childNode != null)
                    {
                        return childNode;
                    }
                }
            }
            return null;
        }
        private RadTreeNode MethodBindingRadTreeViewRecursiveFunction(RadTreeNode p_radTreeNode, int p_selfID, IEnumerable<EntityComplexTreeViewLoadingComplexTable> p_complexTables)
        {
            foreach (var complexTable in p_complexTables.Where(w => w.ParentID.Equals(p_selfID)))
            {

                var radTreeNode = new RadTreeNode();
                radTreeNode.Tag = complexTable.ComplexValue;
                radTreeNode.Text = complexTable.ComplexNameView;
                radTreeNode.Expanded = true;

                if (p_radTreeNode == null) // rootNode
                {
                    radTreeNode.Font = new Font("Tahoma", 9F, FontStyle.Bold);
                    p_radTreeNode = radTreeNode;
                }
                else
                {
                    if (!complexTable.ComplexValue.NodeTypeName.Equals("SHEET"))
                    {
                        radTreeNode.Font = new Font("Tahoma", 8F);
                        radTreeNode.Image = this.MethodBindingRadTreeViewSetNodeImage(complexTable.ComplexValue.NodeTypeName);
                        p_radTreeNode.Nodes.Add(radTreeNode);
                    }
                    else
                    {

                        var tag = p_radTreeNode.Tag as EntityComplexTreeViewLoadingComplexTableComplexValue;
                        if (tag != null)
                        {
                            tag.SheetDynamicID = complexTable.ComplexValue.SheetDynamicID;
                            tag.TotalRowCount = complexTable.ComplexValue.TotalRowCount;
                            //tag.ComplexNameView = p_radTreeNode.Text;
                        }
                        //p_radTreeNode.Text = String.Format("{0} ({1}) "
                        //, p_radTreeNode.Text
                        //, complexTable.ComplexValue.TotalRowCount);
                    }
                }

                this.MethodBindingRadTreeViewRecursiveFunction(radTreeNode, complexTable.SelfID, p_complexTables);
            }

            return p_radTreeNode;
        }
        private Bitmap MethodBindingRadTreeViewSetNodeImage(string p_nodeTypeName)
        {
            return null;
            //switch (p_nodeTypeName.ToUpper())
            //{
            //    case "PLANT":
            //        return Resources.smallRadTreeViewPlant;
            //    case "AREA":
            //        return Resources.smallRadTreeViewArea;
            //    case "UNIT":
            //        return Resources.smallRadTreeViewUnit;
            //    case "SITE":
            //        return Resources.smallRadTreeViewSite;
            //    case "TRAIN":
            //        return Resources.smallRadTreeViewTrain;
            //    case "SHEET":
            //        return Resources.smallRadTreeViewSheet;
            //    default:
            //        return Resources.smallRadTreeViewPlant;
            //} // end switch        
        }
    }
}