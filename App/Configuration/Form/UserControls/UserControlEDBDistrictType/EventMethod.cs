using System;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL.Entities;
using EDCS.WF.BSL.Enums;
using EDCS.WF.BSL.SubForms;
using EDCS.WF.DSL;
using Telerik.WinControls;
using Telerik.WinControls.Data;
using Telerik.WinControls.Enumerations;
using Telerik.WinControls.UI;
using Telerik.WinControls.UI.Data;

namespace Configuration
{
    public partial class UserControlEDBDistrictType
    {
        private void radGridViewDistrictType_CellValidating(object sender, CellValidatingEventArgs e)
        {
            if (e.ColumnIndex > 1 || e.RowIndex > -1) // Delete 아닐 때 (ColumnIndex:-1, RowIndex:-1), 첫번째 Column 제외 (ID값은 추가시 자동으로 부여되기 때문에 없어야 함), 두번째 Column 제외 (Name이 없으면 NameView로 저장)
            {
                if (e.ColumnIndex > 1 && !e.ColumnIndex.Equals(3)) // Reload 아닐 때 (ColumnIndex:-1), 첫번째 Column 제외 (ID값은 추가시 자동으로 부여되기 때문에 없어야 함), 두번째 Column 제외 (Name이 없으면 NameView로 저장)
                {
                    if (e.RowIndex > -1) // Insert 아닐 때(RowIndex:-1)
                    {
                        if (e.Value == null) // 공백 검사
                        {
                            e.Cancel = true;
                            //new CustomRadMessageBoxShowAndTheme(Resources.Message10012).RadMessageBoxShow();
                        }
                    }
                }
            }
        }
        private void radContextMenu1RadMenuItemAdd_Click(object sender, EventArgs e)
        {
            try
            {

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    var entityFrmPopupShareNameView = new EntityFrmPopupShareNameView()
                    {
                        EnumFrmPopupShareNameView = EnumFrmPopupShareNameView.Add,
                        Employee = this._entityEDBInformation.Employee,
                        ListType = this._entityEDBInformation.ListType,
                        RadDropDownStyle = RadDropDownStyle.DropDown,
                    };
                    var frmPopupShareNameView = new FrmPopupShareNameView(entityFrmPopupShareNameView);
                    var result = frmPopupShareNameView.ShowDialog();
                    if ((result.Equals(DialogResult.OK) && frmPopupShareNameView.IsSelectedComboBox && this._entityEDBInformation.ListType.Name.Equals(Enum.GetName(typeof(EnumNameCheck), EnumNameCheck.PowerEquipmentList)))
                        || (result.Equals(DialogResult.OK) && !this._entityEDBInformation.ListType.Name.Equals(Enum.GetName(typeof(EnumNameCheck), EnumNameCheck.PowerEquipmentList))))
                    {
                        var radTreeNode = new RadTreeNode();

                        this.radTreeViewPaint.SelectedNode.Nodes.Add(radTreeNode);

                        int order = 0;
                        int selectedOrder = 0;
                        var node = this.radTreeViewPaint.TopNode;
                        for (int i = 0; i < this.radTreeViewPaint.GetNodeCount(true); i++)
                        {
                            var tag = node.Tag as EntityComplexTreeViewLoadingComplexTableComplexValue;
                            if (tag != null)
                            {
                                tag.DistrictDynamicOrders = i * 100;
                                if (tag.DistrictDynamicID.Equals(0))
                                {
                                    if (node.Parent != null)
                                    {
                                        var parentNode = node.Parent.Tag as EntityComplexTreeViewLoadingComplexTableComplexValue;
                                        if (parentNode != null)
                                        {
                                            tag.DistrictDynamicParentID = parentNode.DistrictDynamicOrders;
                                        }
                                    }
                                }
                            }
                            if (radTreeNode.Equals(node))
                            {
                                order = i * 100;
                            }
                            else if (this.radTreeViewPaint.SelectedNode.Equals(node))
                            {
                                selectedOrder = i * 100;
                            }
                            node = node.NextVisibleNode;
                        } // end for
                        //((EntityComplexTreeViewLoadingComplexTableComplexValue)this.radTreeViewPaint.SelectedNode.Tag).DistrictDynamicOrders = order;
                        this._selectedNodeValue.DistrictDynamicOrders = selectedOrder;

                        var complexValue = new EntityComplexTreeViewLoadingComplexTableComplexValue();
                        //complexValue.SheetDynamicOrders
                        complexValue.DistrictDynamicID = 0;
                        complexValue.DistrictDynamicName = frmPopupShareNameView.NameView;
                        complexValue.DistrictDynamicNameView = frmPopupShareNameView.NameView;
                        complexValue.ListTypeID = this._entityEDBInformation.ListType.ListTypeID;
                        complexValue.ProjectID = this._entityEDBInformation.Project.CaseProjectID;
                        //complexValue.DistrictDynamicOrders = (this.radTreeViewPaint.SelectedNode.Nodes.Count + 1) * 100;
                        complexValue.DistrictDynamicOrders = order;
                        complexValue.DistrictDynamicParentID = this._selectedNodeValue.NodeTypeName.Equals("LISTTYPE") ? 0 : this._selectedNodeValue.DistrictDynamicOrders;
                        complexValue.DistrictTypeOrders = this._selectedNodeValue.DistrictTypeOrders + 100;
                        complexValue.DistrictTypeID = Convert.ToInt32(this.radGridViewDistrictType.Rows[this._selectedNodeValue.DistrictTypeOrders / 100].Cells["DistrictTypeID"].Value);
                        complexValue.DistrictTypeName = this.radGridViewDistrictType.Rows[this._selectedNodeValue.DistrictTypeOrders / 100].Cells["Name"].Value.ToString();
                        complexValue.DistrictTypeNameView = this.radGridViewDistrictType.Rows[this._selectedNodeValue.DistrictTypeOrders / 100].Cells["NameView"].Value.ToString();
                        complexValue.NodeTypeName = this.radGridViewDistrictType.Rows[this._selectedNodeValue.DistrictTypeOrders / 100].Cells["Name"].Value.ToString();
                        complexValue.TotalRowCount = frmPopupShareNameView.TotalTagCount;

                        radTreeNode.Font = new Font("Tahoma", 8F);
                        radTreeNode.Image = this.MethodBindingRadTreeViewSetNodeImage(complexValue.NodeTypeName);
                        radTreeNode.Tag = complexValue;
                        radTreeNode.Text = String.Format("{0} ({1})"
                                    , frmPopupShareNameView.NameView
                                    , complexValue.DistrictTypeNameView);
                        radTreeNode.Expanded = true;

                    }
                    else if (result.Equals(DialogResult.OK) && !frmPopupShareNameView.IsSelectedComboBox && this._entityEDBInformation.ListType.Name.Equals(Enum.GetName(typeof(EnumNameCheck), EnumNameCheck.PowerEquipmentList))) // common System Default와 맵핑 필요
                    {
                        string rUserSystemName = frmPopupShareNameView.NameView;

                        var entityFrmPopupShareNameViewAdd = new EntityFrmPopupShareNameView()
                        {
                            EnumFrmPopupShareNameView = EnumFrmPopupShareNameView.Mapping,
                            Employee = this._entityEDBInformation.Employee,
                            ListType = this._entityEDBInformation.ListType,
                            RadDropDownStyle = RadDropDownStyle.DropDownList,
                        };

                        var frmPopupShareNameViewAdd = new FrmPopupShareNameView(entityFrmPopupShareNameViewAdd);
                        if (frmPopupShareNameViewAdd.ShowDialog().Equals(DialogResult.OK) && frmPopupShareNameViewAdd.IsSelectedComboBox)
                        {
                            var radTreeNode = new RadTreeNode();

                            this.radTreeViewPaint.SelectedNode.Nodes.Add(radTreeNode);

                            int order = 0;
                            int selectedOrder = 0;
                            var node = this.radTreeViewPaint.TopNode;
                            for (int i = 0; i < this.radTreeViewPaint.GetNodeCount(true); i++)
                            {
                                var tag = node.Tag as EntityComplexTreeViewLoadingComplexTableComplexValue;
                                if (tag != null)
                                {
                                    tag.DistrictDynamicOrders = i * 100;
                                    if (tag.DistrictDynamicID.Equals(0))
                                    {
                                        if (node.Parent != null)
                                        {
                                            var parentNode = node.Parent.Tag as EntityComplexTreeViewLoadingComplexTableComplexValue;
                                            if (parentNode != null)
                                            {
                                                tag.DistrictDynamicParentID = parentNode.DistrictDynamicOrders;
                                            }
                                        }
                                    }
                                }
                                if (radTreeNode.Equals(node))
                                {
                                    order = i * 100;
                                }
                                else if (this.radTreeViewPaint.SelectedNode.Equals(node))
                                {
                                    selectedOrder = i * 100;
                                }
                                node = node.NextVisibleNode;
                            } // end for
                            //((EntityComplexTreeViewLoadingComplexTableComplexValue)this.radTreeViewPaint.SelectedNode.Tag).DistrictDynamicOrders = order;
                            this._selectedNodeValue.DistrictDynamicOrders = selectedOrder;

                            var complexValue = new EntityComplexTreeViewLoadingComplexTableComplexValue();
                            //complexValue.SheetDynamicOrders
                            complexValue.DistrictDynamicID = 0;
                            complexValue.DistrictDynamicName = frmPopupShareNameView.NameView;
                            complexValue.DistrictDynamicNameView = frmPopupShareNameView.NameView;
                            complexValue.ListTypeID = this._entityEDBInformation.ListType.ListTypeID;
                            complexValue.ProjectID = this._entityEDBInformation.Project.CaseProjectID;
                            //complexValue.DistrictDynamicOrders = (this.radTreeViewPaint.SelectedNode.Nodes.Count + 1) * 100;
                            complexValue.DistrictDynamicOrders = order;
                            complexValue.DistrictDynamicParentID = this._selectedNodeValue.NodeTypeName.Equals("LISTTYPE") ? 0 : this._selectedNodeValue.DistrictDynamicOrders;
                            complexValue.DistrictTypeOrders = this._selectedNodeValue.DistrictTypeOrders + 100;
                            complexValue.DistrictTypeID = Convert.ToInt32(this.radGridViewDistrictType.Rows[this._selectedNodeValue.DistrictTypeOrders / 100].Cells["DistrictTypeID"].Value);
                            complexValue.DistrictTypeName = this.radGridViewDistrictType.Rows[this._selectedNodeValue.DistrictTypeOrders / 100].Cells["Name"].Value.ToString();
                            complexValue.DistrictTypeNameView = this.radGridViewDistrictType.Rows[this._selectedNodeValue.DistrictTypeOrders / 100].Cells["NameView"].Value.ToString();
                            complexValue.NodeTypeName = this.radGridViewDistrictType.Rows[this._selectedNodeValue.DistrictTypeOrders / 100].Cells["Name"].Value.ToString();
                            complexValue.TotalRowCount = frmPopupShareNameView.TotalTagCount;

                            radTreeNode.Font = new Font("Tahoma", 8F);
                            radTreeNode.Image = this.MethodBindingRadTreeViewSetNodeImage(complexValue.NodeTypeName);
                            radTreeNode.Tag = complexValue;

                            radTreeNode.Text = String.Format("{0} ({1})"
                                        , rUserSystemName
                                        , complexValue.DistrictTypeNameView);
                            radTreeNode.Expanded = true;

                            // System Mapping
                            //var PowerEquipmentSystemMappings = proxy.ReadPowerEquipmentSystemMapping(false, this._project.ProjectID);
                            if (this._powerEquipmentSystemMappings.Where(w => w.NameView.Equals(rUserSystemName)).ToList().Count < 1)
                            {
                                var system = proxy.ReadCommonSystemDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.NameView.Equals(frmPopupShareNameViewAdd.NameView)).SingleOrDefault();
                                if (system != null)
                                {
                                    var powerEquipmentSystemMapping = new EntityEDB_PowerEquipmentSystemMapping();
                                    powerEquipmentSystemMapping.CommonSystemDynamicID = system.CommonSystemDynamicID;
                                    powerEquipmentSystemMapping.ProjectID = this._entityEDBInformation.Project.CaseProjectID;
                                    powerEquipmentSystemMapping.Orders = (this._powerEquipmentSystemMappings.Count + 1) * 100;
                                    powerEquipmentSystemMapping.Name = rUserSystemName;
                                    powerEquipmentSystemMapping.NameView = rUserSystemName;
                                    powerEquipmentSystemMapping.ModifiedDate = DateTime.Now;

                                    _powerEquipmentSystemMappings.Add(powerEquipmentSystemMapping);

                                }
                            }
                        }
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
        private void radContextMenu1RadMenuItemDelete_Click(object sender, EventArgs e)
        {
            if (this.radTreeViewPaint.SelectedNode != null)
            {
                if (RadMessageBox.Show("Do you really want to Delete?", "Delete", MessageBoxButtons.YesNo, RadMessageIcon.Question) == DialogResult.Yes)
                {
                    var parent = this.radTreeViewPaint.SelectedNode.Parent;
                    var systemTag = this.radTreeViewPaint.SelectedNode.Tag as EntityComplexTreeViewLoadingComplexTableComplexValue;

                    string rSystem = systemTag.DistrictDynamicNameView;

                    this.radTreeViewPaint.SelectedNode.Remove();

                    var node = this.radTreeViewPaint.TopNode;

                    for (int i = 0; i < this.radTreeViewPaint.GetNodeCount(true); i++)
                    {
                        var tag = node.Tag as EntityComplexTreeViewLoadingComplexTableComplexValue;
                        if (tag != null)
                        {
                            tag.DistrictDynamicOrders = i * 100;
                            if (tag.DistrictDynamicID.Equals(0))
                            {
                                if (node.Parent != null)
                                {
                                    var parentNode = node.Parent.Tag as EntityComplexTreeViewLoadingComplexTableComplexValue;
                                    if (parentNode != null)
                                    {
                                        tag.DistrictDynamicParentID = parentNode.DistrictDynamicOrders;
                                    }
                                }
                            }
                        }
                        node = node.NextVisibleNode;
                    } // end for

                    if (this._entityEDBInformation.ListType.Name.Equals(Enum.GetName(typeof(EnumNameCheck), EnumNameCheck.PowerEquipmentList)))
                    {
                        var systemMapping = this._powerEquipmentSystemMappings.Where(w => w.NameView.Equals(rSystem)).SingleOrDefault();
                        if (systemMapping != null)
                        {
                            this._powerEquipmentSystemMappings.Remove(systemMapping);
                        }
                    }
                }
            }
        }
        private void radContextMenu1RadMenuItemUpdate_Click(object sender, EventArgs e)
        {
            try
            {

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    if (this.radTreeViewPaint.SelectedNode != null)
                    {
                        var complexValue = (EntityComplexTreeViewLoadingComplexTableComplexValue)this.radTreeViewPaint.SelectedNode.Tag;

                        var entityFrmPopupShareNameView = new EntityFrmPopupShareNameView()
                        {
                            EnumFrmPopupShareNameView = EnumFrmPopupShareNameView.Update,
                            Employee = this._entityEDBInformation.Employee,
                            OldValue = complexValue.DistrictDynamicNameView,
                            ListType = this._entityEDBInformation.ListType,
                        };
                        var frmPopupShareNameView = new FrmPopupShareNameView(entityFrmPopupShareNameView);
                        if (frmPopupShareNameView.ShowDialog().Equals(DialogResult.OK))
                        {
                            complexValue.DistrictDynamicName = frmPopupShareNameView.NameView;
                            complexValue.DistrictDynamicNameView = frmPopupShareNameView.NameView;
                            this.radTreeViewPaint.SelectedNode.Text = String.Format("{0} ({1})"
                                        , frmPopupShareNameView.NameView
                                        , complexValue.DistrictTypeNameView);

                            // this.radTreeViewPaint.SelectedNode.Tag = complexValue;
                            if (this._entityEDBInformation.ListType.Name.Equals(Enum.GetName(typeof(EnumNameCheck), EnumNameCheck.PowerEquipmentList)))
                            {
                                if (!frmPopupShareNameView.IsSelectedComboBox)
                                {
                                    var EquipmentSystemMapping = this._powerEquipmentSystemMappings.Where(w => w.NameView.Equals(entityFrmPopupShareNameView.OldValue)).SingleOrDefault();
                                    if (EquipmentSystemMapping != null)
                                    {
                                        EquipmentSystemMapping.NameView = frmPopupShareNameView.NameView;
                                    }
                                    else
                                    {
                                        string rUserSystemName = frmPopupShareNameView.NameView;

                                        var entityFrmPopupShareNameViewAdd = new EntityFrmPopupShareNameView()
                                        {
                                            EnumFrmPopupShareNameView = EnumFrmPopupShareNameView.Add,
                                            Employee = this._entityEDBInformation.Employee,
                                            ListType = this._entityEDBInformation.ListType,
                                            RadDropDownStyle = RadDropDownStyle.DropDownList,
                                        };

                                        var frmPopupShareNameViewAdd = new FrmPopupShareNameView(entityFrmPopupShareNameViewAdd);
                                        if (frmPopupShareNameViewAdd.ShowDialog().Equals(DialogResult.OK) && frmPopupShareNameViewAdd.IsSelectedComboBox)
                                        {
                                            // System Mapping
                                            //var PowerEquipmentSystemMappings = proxy.ReadPowerEquipmentSystemMapping(false, this._project.ProjectID);
                                            if (this._powerEquipmentSystemMappings.Where(w => w.NameView.Equals(rUserSystemName)).ToList().Count < 1)
                                            {
                                                var system = proxy.ReadCommonSystemDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.NameView.Equals(frmPopupShareNameViewAdd.NameView)).SingleOrDefault();
                                                if (system != null)
                                                {
                                                    var powerEquipmentSystemMapping = new EntityEDB_PowerEquipmentSystemMapping();
                                                    powerEquipmentSystemMapping.CommonSystemDynamicID = system.CommonSystemDynamicID;
                                                    powerEquipmentSystemMapping.ProjectID = this._entityEDBInformation.Project.CaseProjectID;
                                                    powerEquipmentSystemMapping.Orders = (this._powerEquipmentSystemMappings.Count + 1) * 100;
                                                    powerEquipmentSystemMapping.Name = rUserSystemName;
                                                    powerEquipmentSystemMapping.NameView = rUserSystemName;
                                                    powerEquipmentSystemMapping.ModifiedDate = DateTime.Now;

                                                    _powerEquipmentSystemMappings.Add(powerEquipmentSystemMapping);

                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    var EquipmentSystemMapping = _powerEquipmentSystemMappings.Where(w => w.NameView.Equals(entityFrmPopupShareNameView.OldValue)).SingleOrDefault();
                                    if (EquipmentSystemMapping != null)
                                    {
                                        _powerEquipmentSystemMappings.Remove(EquipmentSystemMapping);
                                    }
                                }
                            }
                        }
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
        private void radTreeViewPaint_ContextMenuOpening(object sender, TreeViewContextMenuOpeningEventArgs e)
        {
            if (e.Node == null)
            {
                e.Cancel = true;
                return;
            }

            this._selectedNodeValue = (EntityComplexTreeViewLoadingComplexTableComplexValue)e.Node.Tag;

            var maxDistrictType = this.radGridViewDistrictType.Rows.Count * 100;
            //var maxDistrictType = this._entityDistrictTypes.Max(m => m.Orders);

            //마지막 노드를 클릭한것이라면 추가 비활성화.
            if (this._selectedNodeValue.DistrictTypeOrders.Equals(maxDistrictType))
            {
                e.Menu.Items["radContextMenu1RadMenuItemAdd"].Enabled = false;
                e.Menu.Items["radContextMenu1RadMenuItemUpdate"].Enabled = true;
                e.Menu.Items["radContextMenu1RadMenuItemDelete"].Enabled = true;

                e.Menu.Items["radContextMenu1RadMenuItemAdd"].Text = "Add";
                e.Menu.Items["radContextMenu1RadMenuItemUpdate"].Text = "Rename [" + e.Node.Text + "]";
                e.Menu.Items["radContextMenu1RadMenuItemDelete"].Text = "Delete [" + e.Node.Text + "]";
            }
            //리스트 타입 이면 수정, 삭제 비활성화
            else if (this._selectedNodeValue.NodeTypeName.Equals("LISTTYPE"))
            {
                e.Menu.Items["radContextMenu1RadMenuItemAdd"].Enabled = true;
                e.Menu.Items["radContextMenu1RadMenuItemUpdate"].Enabled = false;
                e.Menu.Items["radContextMenu1RadMenuItemDelete"].Enabled = false;

                e.Menu.Items["radContextMenu1RadMenuItemAdd"].Text = "Add [" + this.radGridViewDistrictType.Rows[0].Cells["NameView"].Value.ToString() + "]";
                e.Menu.Items["radContextMenu1RadMenuItemUpdate"].Text = "Rename";
                e.Menu.Items["radContextMenu1RadMenuItemDelete"].Text = "Delete";
            }
            //
            else
            {
                e.Menu.Items["radContextMenu1RadMenuItemAdd"].Enabled = true;
                e.Menu.Items["radContextMenu1RadMenuItemUpdate"].Enabled = true;
                e.Menu.Items["radContextMenu1RadMenuItemDelete"].Enabled = true;

                e.Menu.Items["radContextMenu1RadMenuItemAdd"].Text = "Add [" + this.radGridViewDistrictType.Rows[this.radGridViewDistrictType.Rows.Count - 1].Cells["NameView"].Value.ToString() + "]";
                e.Menu.Items["radContextMenu1RadMenuItemUpdate"].Text = "Rename [" + e.Node.Text + "]";
                e.Menu.Items["radContextMenu1RadMenuItemDelete"].Text = "Delete [" + e.Node.Text + "]";
            }

            //switch (this._selectedNodeValue.NodeTypeName)
            //{
            //    case "LISTTYPE":
            //        e.Menu.Items["radContextMenu1RadMenuItemUpdate"].Enabled = false;
            //        e.Menu.Items["radContextMenu1RadMenuItemAdd"].Enabled = true;
            //        e.Menu.Items["radContextMenu1RadMenuItemDelete"].Enabled = false;
            //        e.Menu.Items["radContextMenu1RadMenuItemAddSheet"].Enabled = false;
            //        e.Menu.Items["radContextMenu1RadMenuItemDataCopy"].Enabled = false;
            //        e.Menu.Items["radContextMenu1RadMenuItemDataPaste"].Enabled = false;
            //        break;
            //    case "SHEET":
            //        e.Menu.Items["radContextMenu1RadMenuItemAdd"].Enabled = false;
            //        e.Menu.Items["radContextMenu1RadMenuItemUpdate"].Enabled = true;
            //        e.Menu.Items["radContextMenu1RadMenuItemDelete"].Enabled = true;
            //        e.Menu.Items["radContextMenu1RadMenuItemAddSheet"].Enabled = false;
            //        if (this._listType.Name.Equals(Enum.GetName(typeof(EDCS.WF.BSL.Enums.EnumNameCheck), EDCS.WF.BSL.Enums.EnumNameCheck.HeatMaterialBalance)))
            //        {
            //            e.Menu.Items["radContextMenu1RadMenuItemDataCopy"].Enabled = false;
            //            e.Menu.Items["radContextMenu1RadMenuItemDataPaste"].Enabled = false;
            //        }
            //        else
            //        {
            //            e.Menu.Items["radContextMenu1RadMenuItemDataCopy"].Enabled = true;
            //            if (!this._copySheetID.Equals(0))
            //            {
            //                e.Menu.Items["radContextMenu1RadMenuItemDataPaste"].Enabled = true;
            //            }
            //            else
            //            {
            //                e.Menu.Items["radContextMenu1RadMenuItemDataPaste"].Enabled = false;
            //            }
            //        }

            //        break;
            //    default:
            //        if (this.MethodEnabledCheckAddDefault())
            //        {
            //            e.Menu.Items["radContextMenu1RadMenuItemAdd"].Enabled = true;
            //            e.Menu.Items["radContextMenu1RadMenuItemAddSheet"].Enabled = false;
            //        }
            //        else
            //        {
            //            e.Menu.Items["radContextMenu1RadMenuItemAdd"].Enabled = false;
            //            e.Menu.Items["radContextMenu1RadMenuItemAddSheet"].Enabled = true;
            //        }
            //        e.Menu.Items["radContextMenu1RadMenuItemUpdate"].Enabled = true;
            //        e.Menu.Items["radContextMenu1RadMenuItemDelete"].Enabled = true;
            //        e.Menu.Items["radContextMenu1RadMenuItemDataCopy"].Enabled = false;
            //        e.Menu.Items["radContextMenu1RadMenuItemDataPaste"].Enabled = false;
            //        break;
            //} // end switch
        }
        private void radTreeViewPaint_DragEnded(object sender, RadTreeViewDragEventArgs e)
        {
            var node = this.radTreeViewPaint.TopNode;
            for (int i = 0; i < this.radTreeViewPaint.GetNodeCount(true); i++)
            {
                var tag = node.Tag as EntityComplexTreeViewLoadingComplexTableComplexValue;
                if (tag != null)
                {
                    tag.DistrictDynamicOrders = i * 100;
                    //if (tag.DistrictDynamicID.Equals(0))
                    {
                        if (node.Parent != null)
                        {
                            var parentNode = node.Parent.Tag as EntityComplexTreeViewLoadingComplexTableComplexValue;
                            if (parentNode != null)
                            {
                                if (tag.DistrictDynamicID.Equals(0))
                                {
                                    tag.DistrictDynamicParentID = parentNode.DistrictDynamicOrders;
                                }
                                else
                                {
                                    tag.DistrictDynamicParentID = parentNode.DistrictDynamicID;
                                }
                            }
                        }
                    }
                }
                node = node.NextVisibleNode;
            }
        }
        private void radTreeViewPaint_DragEnding(object sender, RadTreeViewDragCancelEventArgs e)
        {
            if (e.Node.Level != e.TargetNode.Level || e.DropPosition == DropPosition.AsChildNode)
            {
                e.Cancel = true;
            }
        }
        private void radTreeViewPaint_NodeExpandedChanging(object sender, RadTreeViewCancelEventArgs e)
        {
            foreach (var node in e.Node.Nodes)
            {
                node.Font = new Font("Tahoma", 8F);

                string nodeTypeName = node.Name.Trim().ToUpper();
                //switch (nodeTypeName.ToUpper())
                //{
                //    case "PLANT":
                //        node.Image = Resources.smallRadTreeViewPlant;
                //        break;
                //    case "AREA":
                //        node.Image = Resources.smallRadTreeViewArea;
                //        break;
                //    case "UNIT":
                //        node.Image = Resources.smallRadTreeViewUnit;
                //        break;
                //    case "SITE":
                //        node.Image = Resources.smallRadTreeViewSite;
                //        break;
                //    case "TRAIN":
                //        node.Image = Resources.smallRadTreeViewTrain;
                //        break;
                //    default:
                //        node.Image = Resources.smallRadTreeViewPlant;
                //        break;
                //} // end switch
            } // end foreach
        }
        private void radTreeViewPaint_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyValue.Equals(46)) // DELETE
            {
                this.radContextMenu1RadMenuItemDelete_Click(null, null);
            }
            else if (e.KeyValue.Equals(113)) // F2 - Update
            {
                this.radContextMenu1RadMenuItemUpdate_Click(null, null);
            }

        }
        private void radGridViewDistrictType_RowsChanged(object sender, GridViewCollectionChangedEventArgs e)
        {
            //  this.radGridViewDistrictType.IsDisplayed
            //  this.radGridViewDistrictType.IsHandleCreated
            //  this.radGridViewDistrictType.IsLoaded

            if (this._flagLoading)
            {
                this.radGridViewDistrictType.RowsChanged -= new GridViewCollectionChangedEventHandler(radGridViewDistrictType_RowsChanged);

                //this.radGridViewDistrictType.RowsChanged = null;
                if (e.Action.Equals(NotifyCollectionChangedAction.Add)) // 추가
                {
                    ((GridViewRowInfo)e.NewItems[0]).Cells[0].Value = this._originDistrictTypeID;
                    if (((GridViewRowInfo)e.NewItems[0]).Cells[2].Value == null)
                    {
                        ((GridViewRowInfo)e.NewItems[0]).Cells[2].Value = this._originDistrictTypeID;
                    }
                    if (((GridViewRowInfo)e.NewItems[0]).Cells[1].Value == null)
                    {
                        ((GridViewRowInfo)e.NewItems[0]).Cells[1].Value = ((GridViewRowInfo)e.NewItems[0]).Cells[2].Value;
                    }
                    ((GridViewRowInfo)e.NewItems[0]).Cells[3].Value = false;
                    this._originDistrictTypeID++;
                    //this._entityDistrictTypes.Add(new EntityDistrictType()
                    //    {
                    //        DistrictTypeID = 0,
                    //        ProjectID = this._entityFrmDistrictType.Project.ProjectID,
                    //        ListTypeID = this._entityFrmDistrictType.ListType.ListTypeID,
                    //        VisibleFlag = true,
                    //        DeleteFlag = false,
                    //        ModifiedDate = DateTime.Now,
                    //        Orders = (this.radGridViewDistrictType.Rows.Count) * 100,
                    //        Name = (((Telerik.WinControls.UI.GridViewRowInfo)e.NewItems[0]).Cells["NameView"].Value).ToString(),
                    //        NameView = (((Telerik.WinControls.UI.GridViewRowInfo)e.NewItems[0]).Cells["NameView"].Value).ToString(),
                    //        //NameView = this.radGridViewDistrictType.Rows[this.radGridViewDistrictType.Rows.Count - 1].Cells["NameView"].Value == null ? "" : this.radGridViewDistrictType.Rows[this.radGridViewDistrictType.Rows.Count - 1].Cells["NameView"].Value.ToString(),
                    //    });
                    //this.MethodBindingRadTreeView();
                }
                else if (e.Action.Equals(NotifyCollectionChangedAction.Remove)) // 삭제
                {
                    int districtTypeID = Convert.ToInt32(((GridViewRowInfo)e.OldItems[0]).Cells[0].Value);
                    this.MethodDeleteDistrictType(districtTypeID, this.radTreeViewPaint.Nodes);

                    var node = this.radTreeViewPaint.TopNode;
                    for (int i = 0; i < this.radTreeViewPaint.GetNodeCount(true); i++)
                    {
                        var tag = node.Tag as EntityComplexTreeViewLoadingComplexTableComplexValue;
                        if (tag != null)
                        {
                            tag.DistrictDynamicOrders = (i + 1) * 100;
                        }
                        node = node.NextVisibleNode;
                    } // end for

                    var count = this.radGridViewDistrictType.Rows.Count;
                    for (int i = e.OldStartingIndex; i < count; i++)
                    {
                        this.radGridViewDistrictType.Rows[e.OldStartingIndex].Delete();
                    }
                }
                else if (e.Action.Equals(NotifyCollectionChangedAction.ItemChanged)) // 수정
                {
                    if (((GridViewRowInfo)e.NewItems[0]).Index > -1)
                    //if (e.OldItems != null)
                    {
                        int districtTypeID = Convert.ToInt32(((GridViewRowInfo)e.NewItems[0]).Cells[0].Value);
                        this.MethodUpdateDistrictType(districtTypeID, (((GridViewRowInfo)e.NewItems[0]).Cells["NameView"].Value).ToString(), this.radTreeViewPaint.Nodes);

                        //var nodes = this.radTreeViewPaint.Nodes.Where(w => ((EntityComplexTreeViewLoadingComplexTableComplexValue)w.Tag).DistrictTypeID.Equals(districtTypeID));
                        //foreach (var node in nodes)
                        //{
                        //    ((EntityComplexTreeViewLoadingComplexTableComplexValue)node.Tag).DistrictTypeName = (((Telerik.WinControls.UI.GridViewRowInfo)e.NewItems[0]).Cells["NameView"].Value).ToString();
                        //    ((EntityComplexTreeViewLoadingComplexTableComplexValue)node.Tag).DistrictTypeNameView = (((Telerik.WinControls.UI.GridViewRowInfo)e.NewItems[0]).Cells["NameView"].Value).ToString();

                        //    node.Text = String.Format("{0} ({1})"
                        //        , ((EntityComplexTreeViewLoadingComplexTableComplexValue)node.Tag).DistrictDynamicNameView
                        //        , (((Telerik.WinControls.UI.GridViewRowInfo)e.NewItems[0]).Cells["NameView"].Value).ToString());
                        //}
                    }
                }
                else if (e.Action.Equals(NotifyCollectionChangedAction.Move)) // 이동
                {
                    //if (!e.OldStartingIndex.Equals(e.NewStartingIndex))
                    //{
                    //    if (proxy.ReadEnableMoveChangeCheck(false, this._project.ProjectID, this._listType.ListTypeID))
                    //    {
                    //        this.MethodBindingRadTreeView();
                    //    }
                    //    else
                    //    {
                    //        this.radGridViewDistrictType.RowsChanged -= new Telerik.WinControls.UI.GridViewCollectionChangedEventHandler(radGridViewDistrictType_RowsChanged);
                    //        this.radGridViewDistrictType.Rows.Move(e.NewStartingIndex, e.OldStartingIndex);
                    //        this.radGridViewDistrictType.RowsChanged += new Telerik.WinControls.UI.GridViewCollectionChangedEventHandler(radGridViewDistrictType_RowsChanged);

                    //        new EDCS.WF.BSL.Helpers.CustomRadMessageBoxShowAndTheme(EDCS.WF.Properties.Resources.Message10024, this._entityFrmDistrictType.CommonThemeName).RadMessageBoxShow();

                    //        var entityFrmDistrictDynamic = new EDCS.WF.BSL.Entities.EntityFrmDistrictDynamic()
                    //        {
                    //            CommonThemeName = this._entityFrmDistrictType.CommonThemeName,
                    //            Employee = this._entityFrmDistrictType.Employee,
                    //            Project = this._project,
                    //            ListType = this._listType,
                    //        };
                    //        new EDCS.WF.SubForms.FrmDistrictDynamic(entityFrmDistrictDynamic).ShowDialog();
                    //    }
                    //}
                }
                this.radGridViewDistrictType.RowsChanged += new GridViewCollectionChangedEventHandler(radGridViewDistrictType_RowsChanged);
            }
        }
        private void radGridViewDistrictType_RowsChanging(object sender, GridViewCollectionChangingEventArgs e)
        {
            if (e.Action.Equals(NotifyCollectionChangedAction.Remove)) // 삭제
            {
                if (RadMessageBox.Show("Do you really want to Delete?", "Delete", MessageBoxButtons.YesNo, RadMessageIcon.Question) == DialogResult.No)
                {
                    e.Cancel = true;
                }
            }
        }
        private void radDropDownListListType_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            if (this.radDropDownListListType.SelectedItem != null)
            {
                this._entityEDBInformation.ListType = (EntityEDB_ListType)this.radDropDownListListType.SelectedItem.DataBoundItem;

                if ((this._entityEDBInformation.Project != null) && (this._entityEDBInformation.ListType != null))
                {
                    this.MethodSettingRadGridViewDistrictType();
                }
            }
        }
        private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            if (this.radDropDownListProject.SelectedItem != null)
            {
                this._entityEDBInformation.Project = (EntityEDB_Project)this.radDropDownListProject.SelectedItem.DataBoundItem;

                if ((this._entityEDBInformation.Project != null) && (this._entityEDBInformation.ListType != null))
                {
                    this.MethodSettingRadGridViewDistrictType();
                }
            }
        }
        private void radTreeViewPaint_SelectedNodeChanged(object sender, RadTreeViewEventArgs e)
        {
            if (e.Node != null)
            {
                this._selectedNodeValue = (EntityComplexTreeViewLoadingComplexTableComplexValue)e.Node.Tag;
            }
        }
        private void radGridViewDistrictType_ValueChanged(object sender, EventArgs e)
        {
            this.radGridViewDistrictType.ValueChanged -= new EventHandler(radGridViewDistrictType_ValueChanged);
            this.radGridViewDistrictType.RowsChanged -= new GridViewCollectionChangedEventHandler(radGridViewDistrictType_RowsChanged);
            if (this.radGridViewDistrictType.ActiveEditor is RadCheckBoxEditor)
            {
                if (this.radGridViewDistrictType.SelectedRows.Count > 0)
                {
                    if (this.radGridViewDistrictType.RowCount.Equals(this.radGridViewDistrictType.SelectedRows[0].Index + 1))
                    {
                        this.radGridViewDistrictType.ActiveEditor.Value = ToggleState.Off;
                    }
                    else
                    {
                        foreach (var cell in this.radGridViewDistrictType.Rows.Where(w => Convert.ToBoolean(w.Cells["RevisionUnit"].Value).Equals(true)).Select(s => s.Cells["RevisionUnit"]))
                        {
                            cell.Value = false;
                        } // end foreach
                    }
                }
            }
            this.radGridViewDistrictType.RowsChanged += new GridViewCollectionChangedEventHandler(radGridViewDistrictType_RowsChanged);
            this.radGridViewDistrictType.ValueChanged += new EventHandler(radGridViewDistrictType_ValueChanged);
        }
        void radDropDownListProject_SelectedIndexChanging(object sender, PositionChangingCancelEventArgs e)
        {
            var newProject = (EntityEDB_Project)this.radDropDownListProject.Items[e.Position].DataBoundItem;
            var oldProject = (EntityEDB_Project)this.radDropDownListProject.SelectedItem.DataBoundItem;
            if (!newProject.ListTypeFlag.Contains(this._entityEDBInformation.ListType.ListTypeID))
            {
                //e.Cancel = true;
            }
            //
        }
        void radDropDownListListType_SelectedIndexChanging(object sender, PositionChangingCancelEventArgs e)
        {
            var newListType = (EntityEDB_ListType)this.radDropDownListListType.Items[e.Position].DataBoundItem;
            var oldListType = (EntityEDB_ListType)this.radDropDownListListType.SelectedItem.DataBoundItem;
            if (!this._entityEDBInformation.Project.ListTypeFlag.Contains(newListType.ListTypeID))
            {
                e.Cancel = true;
            }

            //e.Cancel
        }
    }
}