using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.Model;
using Telerik.WinControls.UI.Data;
using Configuration.Properties;
using EDCS.WF.DSL.Entities;
using IDCS.ServiceImpl;
using Configuration.BSL.Enums;
using EDCS.WF.BSL.Enums;
using EDCS.WF.BSL.Entities;
using EDCS.WF.BSL;
using CommonUtil.CommonLib;
using EDCS.WF.BSL.Helpers;
using EDCS.WF.BSL.Method;



namespace Configuration
{
    public partial class UserControlDocNoMapping : UserControlBase
    {
        private enum enColumn { DocNo, DocTitle, Last };
        private int[] mColumnWidth = new int[] { 120, 180 };
        private string[] mColumnName = new string[] { "DocNo", "DocTitle" };
        private bool[] mColumnLock = new bool[] { true, true };

        private enum enColumn_Menu { DistricDynamicID, UnitName, Last };
        private int[] mColumnWidth_Menu = new int[] { 0, 150 };
        private string[] mColumnName_Menu = new string[] { "DistricDynamicID", "UnitName", };
        private bool[] mColumnLock_Menu = new bool[] { true, true };

        int SelectRow;

        public UserControlDocNoMapping()
        {
            InitializeComponent();
            Telerik.WinControls.UI.RadPageViewStripElement stripElement = (Telerik.WinControls.UI.RadPageViewStripElement)this.radPageViewDoc.ViewElement;
            stripElement.StripButtons = Telerik.WinControls.UI.StripViewButtons.None;

        }
        public override void LoadData()
        {
            radDropDownListBinding();
            InitSpread_DocNo();
            InitSpread_Unit();
            if (this.radDropDownListProject.Items.Count > 0)
            {
                this.radDropDownListProject_SelectedIndexChanged(null, null);
            }
        }

        #region Control Setting & Binding
        private void radDropDownListBinding()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                //var projects = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true && w.EnsembleFlag.Equals(true)).OrderByDescending(ob => ob.No).ToList();
                var projects = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true && (w.EnsembleType.Equals((int)EnumEnsembleType.All) || w.EnsembleType.Equals((int)EnumEnsembleType.CommentCheck))).OrderByDescending(ob => ob.No).ToList();
                projects.ConvertAll(ca => ca.ComplexNameView = ca.ComplexNameView + " - " + ca.Type);
                this.radDropDownListProject.Items.Clear();
                this.radDropDownListProject.ValueMember = "ProjectMasterID";
                this.radDropDownListProject.DisplayMember = "ComplexNameView";
                this.radDropDownListProject.SelectedValue = this._entityEDBInformation.Project.ProjectMasterID;
                this.radDropDownListProject.DataSource = projects;

                this.radDropDownListListType.Items.Clear();
                var lstListTypes = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.ListTypeID.Equals((int)EnumNameCheck.LineList) || w.ListTypeID.Equals((int)EnumNameCheck.ProcessEquipmentList)
                    || w.ListTypeID.Equals((int)EnumNameCheck.InstrumentIndex)).ToList();
                this.radDropDownListListType.ValueMember = "ListTypeID";
                this.radDropDownListListType.DisplayMember = "ComplexNameView";
                this.radDropDownListListType.DataSource = lstListTypes;

                this.radDropDownListProject.SelectedIndexChanged += new PositionChangedEventHandler(this.radDropDownListProject_SelectedIndexChanged);

                this.radDropDownListListType.SelectedIndexChanged += new PositionChangedEventHandler(this.radDropDownListListType_SelectedIndexChanged);
            }
        }

        void radDropDownListProject_SelectedIndexChanged(object sender, PositionChangedEventArgs e)
        {
            if (radDropDownListProject.SelectedItem != null && radDropDownListProject.SelectedItem.Value != null)
            {
                var project = (EntityEDB_Project)this.radDropDownListProject.SelectedItem.DataBoundItem;
                var ListTypeID = int.Parse(this.radDropDownListListType.SelectedValue.ToString());
                SetSpread_DocNo();
                SetSpread_Unit(project.ProjectID, ListTypeID);
                this.TreeViewBinding(project.ProjectID, ListTypeID);
            }
        }

        private void radDropDownListListType_SelectedIndexChanged(object sender, PositionChangedEventArgs e)
        {
            if (this.radDropDownListListType.SelectedItem != null && this.radDropDownListListType.SelectedItem.Value != null)
            {
                var project = (EntityEDB_Project)this.radDropDownListProject.SelectedItem.DataBoundItem;
                var ListTypeID = int.Parse(this.radDropDownListListType.SelectedValue.ToString());
                SetSpread_DocNo();
                SetSpread_Unit(project.ProjectID, ListTypeID);
            }
        }

        private void InitSpread_DocNo()
        {
            SpreadUtil.InitSpread(fpSpreadDocNo, mColumnWidth, mColumnName, mColumnLock, null, null, null
               , null, null, Color.LightGray, true, true);
        }

        private void InitSpread_Unit()
        {
            SpreadUtil.InitSpread(this.fpSpreadNotAvailableUnit, mColumnWidth_Menu, mColumnName_Menu, mColumnLock_Menu, null, null, null, null, null, Color.LightGray, true, true);

            SpreadUtil.InitSpread(this.fpSpreadAvailableUnit, mColumnWidth_Menu, mColumnName_Menu, mColumnLock_Menu, null, null, null, null, null, Color.LightGray, true, true);

            //fpSpreadNotAvailableUnit.ActiveSheet.Columns[(int)enColumn_Menu.Orders].HorizontalAlignment = CellHorizontalAlignment.Center;
            //fpSpreadAvailableUnit.ActiveSheet.Columns[(int)enColumn_Menu.Orders].HorizontalAlignment = CellHorizontalAlignment.Center;
        }

        private void SetSpread_DocNo() //Ensemble+ 리스트별 DocumentNo SpreadSetting
        {
            InitSpread_DocNo();
            string sEnsembleInstance = string.Empty;
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                string EnsembleList = string.Empty;
                EntityEnsembleList List = new EntityEnsembleList();
                if (this.radDropDownListListType.SelectedValue.Equals((int)EnumNameCheck.LineList))
                {
                    EnsembleList = List.Line;
                }
                else if (this.radDropDownListListType.SelectedValue.Equals((int)EnumNameCheck.ProcessEquipmentList))
                {
                    EnsembleList = List.Process;
                }
                else if (this.radDropDownListListType.SelectedValue.Equals((int)EnumNameCheck.InstrumentIndex))
                {
                    EnsembleList = List.Instrument;
                }
                var Project = (EntityEDB_Project)this.radDropDownListProject.SelectedItem.DataBoundItem;

                sEnsembleInstance = "getMYTeamProjectDoc";
                object[] fParameter = { Project.No, EnsembleList };
                string sEnsembleWebSerVice = WcfHelper.MethodGetEnsembleWebReferenceUrl(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                WebServiceInterface fWebServiceInterface = new WebServiceInterface(sEnsembleWebSerVice);
                var docNos = fWebServiceInterface.InvokeMethod(sEnsembleInstance, fParameter);
                if (!string.IsNullOrWhiteSpace(docNos.ToString()))
                {
                    string[] DocNos = docNos.ToString().Split('|');
                    int iRowCount = 0;

                    for (int i = 0; i < DocNos.Length; i++)
                    {
                        string[] DocNo = DocNos[i].Split('^');

                        iRowCount++;
                        this.fpSpreadDocNo.ActiveSheet.Rows.Count = iRowCount;

                        this.fpSpreadDocNo.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.DocNo].Value = DocNo[0];
                        this.fpSpreadDocNo.ActiveSheet.Cells[iRowCount - 1, (int)enColumn.DocTitle].Value = DocNo[2];
                    }
                }
            }
        }

        private void SetSpread_Unit(int p_projectId, int p_listtypeId) // DocumentNo와 Mapping 될 Unit SpreadSetting 
        {
            InitSpread_Unit();

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                int iRowCount_NotUse = 0;
                int iRowCount_Use = 0;

                var DocumentNoMappings = proxy.ReadCommonDocumentNoMappings(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, (long)this.radDropDownListProject.SelectedValue, p_listtypeId);
                var DistricDynamics = proxy.ReadDistrictDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, p_projectId, p_listtypeId);
                DistricDynamics.RemoveAll(rm => rm.DistrictTypeName.Equals("1") || rm.DistrictTypeName.ToUpper().Equals("AREA"));

                if (DocumentNoMappings.Count > 0)  //DocumentNo 별로 Mapping 된 유닛을 표현하고 나머지 Unit만 AvailableUnit에 Setting 하기 위함.
                {
                    if (DocumentNoMappings.Where(w => w.DocumentNo.Equals(this.fpSpreadDocNo.ActiveSheet.Cells[SelectRow, (int)enColumn.DocNo].Text)).Count() > 0)
                    {
                        foreach (var documentNoMapping in DocumentNoMappings)
                        {
                            if (documentNoMapping.DocumentNo.Equals(this.fpSpreadDocNo.ActiveSheet.Cells[SelectRow, (int)enColumn.DocNo].Text))
                            {
                                iRowCount_Use++;
                                this.fpSpreadAvailableUnit.ActiveSheet.Rows.Count = iRowCount_Use;
                                this.fpSpreadAvailableUnit.ActiveSheet.Cells[iRowCount_Use - 1, (int)enColumn_Menu.DistricDynamicID].Text = Util.NullToSpace(documentNoMapping.DistricDynamicID);
                                this.fpSpreadAvailableUnit.ActiveSheet.Cells[iRowCount_Use - 1, (int)enColumn_Menu.UnitName].Text = Util.NullToSpace(documentNoMapping.DistricDynamicNameView.ToString());
                            }

                            DistricDynamics.RemoveAll(rm => rm.DistrictDynamicID.Equals(documentNoMapping.DistricDynamicID));
                        }

                        foreach (var districdynamic in DistricDynamics)
                        {
                            iRowCount_NotUse++;
                            fpSpreadNotAvailableUnit.ActiveSheet.Rows.Count = iRowCount_NotUse;

                            fpSpreadNotAvailableUnit.ActiveSheet.Cells[iRowCount_NotUse - 1, (int)enColumn_Menu.DistricDynamicID].Text = Util.NullToSpace(districdynamic.DistrictDynamicID);
                            fpSpreadNotAvailableUnit.ActiveSheet.Cells[iRowCount_NotUse - 1, (int)enColumn_Menu.UnitName].Text = Util.NullToSpace(districdynamic.NameView.ToString());
                        }
                    }
                    else // Mapping된 Unit이 없을 경우 NotAvailable에 전체 유닛 Setting
                    {
                        foreach (var documentNoMapping in DocumentNoMappings)
                        {
                            DistricDynamics.RemoveAll(rm => rm.DistrictDynamicID.Equals(documentNoMapping.DistricDynamicID));
                        }
                        foreach (var districdynamic in DistricDynamics)
                        {
                            iRowCount_NotUse++;
                            fpSpreadNotAvailableUnit.ActiveSheet.Rows.Count = iRowCount_NotUse;

                            fpSpreadNotAvailableUnit.ActiveSheet.Cells[iRowCount_NotUse - 1, (int)enColumn_Menu.DistricDynamicID].Text = Util.NullToSpace(districdynamic.DistrictDynamicID);
                            fpSpreadNotAvailableUnit.ActiveSheet.Cells[iRowCount_NotUse - 1, (int)enColumn_Menu.UnitName].Text = Util.NullToSpace(districdynamic.NameView.ToString());
                        }
                    }
                }
                else
                {
                    //Mapping 된 유닛이 존재하지 않을경우
                    foreach (var districdynamic in DistricDynamics)
                    {
                        iRowCount_NotUse++;
                        fpSpreadNotAvailableUnit.ActiveSheet.Rows.Count = iRowCount_NotUse;

                        fpSpreadNotAvailableUnit.ActiveSheet.Cells[iRowCount_NotUse - 1, (int)enColumn_Menu.DistricDynamicID].Text = Util.NullToSpace(districdynamic.DistrictDynamicID);
                        fpSpreadNotAvailableUnit.ActiveSheet.Cells[iRowCount_NotUse - 1, (int)enColumn_Menu.UnitName].Text = Util.NullToSpace(districdynamic.NameView.ToString());
                    }
                }
            }
        }

        private void TreeViewBinding(int p_projectId, int p_listtypeId)
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                //var DistrictDynamics = proxy.ReadDistrictDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, p_projectId, p_listtypeId);
                var DistrictDynamics = proxy.ReadDistrictDynamicsAll(EDCS.WF.BSL.Variable.SharedVariable.IsIE, p_projectId, 0);
                //var DocumentNoMappings = proxy.ReadCommonDocumentNoMappings(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, (long)this.radDropDownListProject.SelectedValue, p_listtypeId);
                var DocumentNoMappings = proxy.ReadCommonDocumentNoMappingsbyProjectMasterId(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, (long)this.radDropDownListProject.SelectedValue);
                SetTreeView_EDB(ToolType.LineList, this.radTreeViewLine, DocumentNoMappings, DistrictDynamics.Where(w => w.ListTypeID.Equals((int)EnumNameCheck.LineList)).ToList());
                SetTreeView_EDB(ToolType.ProcessEquipmentList, this.radTreeViewProcess, DocumentNoMappings, DistrictDynamics.Where(w => w.ListTypeID.Equals((int)EnumNameCheck.ProcessEquipmentList)).ToList());
                SetTreeView_EDB(ToolType.InstrumentIndex, this.radTreeViewInstrument, DocumentNoMappings, DistrictDynamics.Where(w => w.ListTypeID.Equals((int)EnumNameCheck.InstrumentIndex)).ToList());
            }
        }

        private void SetTreeView_EDB(string sToolType, Telerik.WinControls.UI.RadTreeView radTreeViewItem, List<EntityCommon_DocumentNoMapping> p_documentNomapping, List<EntityEDB_DistrictDynamic> p_districtdynamics)
        {
            radTreeViewItem.Nodes.Clear();
            radTreeViewItem.Font = Information.FONT_REGULAR;
            Telerik.WinControls.UI.RadTreeNode node = new Telerik.WinControls.UI.RadTreeNode();
            if (p_districtdynamics != null)
            {
                string sPBSMappingLevel_Area = string.Empty;
                string sPBSMappingLevel_Unit = string.Empty;

                if (p_districtdynamics != null)
                {
                    if (sToolType == ToolType.ProcessEquipmentList)
                    {
                        var vArea = p_districtdynamics.Where(w => w.Orders == 100).FirstOrDefault();
                        var vUnit = p_districtdynamics.Where(w => w.Orders == 200).FirstOrDefault();

                        if (vArea != null)
                            sPBSMappingLevel_Area = vArea.NameView;

                        if (vUnit != null)
                            sPBSMappingLevel_Unit = vUnit.NameView;
                    }
                    else
                    {
                        var vArea = p_districtdynamics.Where(w => w.Orders == 100).FirstOrDefault();
                        var vUnit = p_districtdynamics.Where(w => w.Orders == 200).FirstOrDefault();

                        if (vArea != null)
                            sPBSMappingLevel_Area = vArea.NameView;

                        if (vUnit != null)
                            sPBSMappingLevel_Unit = vUnit.NameView;
                    }
                }

                Telerik.WinControls.UI.RadTreeNode nodParent = new Telerik.WinControls.UI.RadTreeNode();
                //20170207
                foreach (EntityEDB_DistrictDynamic districtDynamic in p_districtdynamics)
                {
                    if (districtDynamic.ParentID == 0)
                    {
                        node = new Telerik.WinControls.UI.RadTreeNode();
                        node.Font = Information.FONT_REGULAR;
                        node.Image = Resources.smallRed;
                        node.Tag = districtDynamic.DistrictDynamicID;
                        node.Text = string.Format("{0}", districtDynamic.NameView);
                        nodParent = radTreeViewItem.GetNodeByName(districtDynamic.ParentID.ToString());
                    }
                    else
                    {
                        node = new Telerik.WinControls.UI.RadTreeNode();
                        node.Font = Information.FONT_REGULAR;
                        node.Image = Resources.smallGreen;
                        node.Text = string.Format("{0} - [{1}]", districtDynamic.NameView, p_documentNomapping.Where(w => w.DistricDynamicID.Equals(districtDynamic.DistrictDynamicID)).Select(s => s.DocumentNo).SingleOrDefault());
                    }

                    string sDistrictTypeOrders = districtDynamic.DistrictTypeOrders.ToString();

                    if (nodParent == null)
                    {
                        radTreeViewItem.Nodes.Add(node);
                    }
                    else
                    {
                        nodParent.Nodes.Add(node);
                    }
                }
            }
            else
            {
                node = new Telerik.WinControls.UI.RadTreeNode();
                node.Font = Information.FONT_REGULAR;
                node.Name = " ";
                node.Text = "cannot find PBS";
                node.Image = Resources.smallRed;
                radTreeViewItem.Nodes.Add(node);
            }
            radTreeViewItem.ExpandAll();
        }
        //
        private void SetPBSColor(List<EntityCommon_DocumentNoMapping> p_districtdyanmics, string sToolType, string sNodeType, string sNodeName, string sNodeText)
        {
            List<EntityCommon_DocumentNoMapping> DocumentNoMapping = new List<EntityCommon_DocumentNoMapping>();

            //if (sToolType == ToolType.ProcessEquipmentList)
            //{
            //    DocumentNoMapping = p_districtdyanmics.Where(w => w.ListTypeID == sNodeType && w.DistrictDynamicID == int.Parse(sNodeName)).ToList();
            //}
            //if (sToolType == ToolType.LineList)
            //{
            //    DocumentNoMapping = p_districtdyanmics.Where(w => w.ListTypeID == sNodeType && w.DistrictDynamicID == int.Parse(sNodeName)).ToList();
            //}

            //SetColor(DocumentNoMapping, sNodeName, sNodeText, ref node);

            DocumentNoMapping.Clear();
        }

        private void SetColor(List<EntityCommon_DocumentNoMapping> p_documentmappings, string sNodeName, string sNodeText, ref Telerik.WinControls.UI.RadTreeNode node)
        {
            Dictionary<string, bool> dicMapping = new Dictionary<string, bool>();

            dicMapping.Add("ProcessEquipment", false);
            dicMapping.Add("LineList", false);

            node.Image = Resources.smallRed;
            node.Name = sNodeName;

            string sName = string.Empty;

            List<string> lstName = new List<string>();

            if (p_documentmappings.Count > 0)
            {
                foreach (EntityCommon_DocumentNoMapping cpbs in p_documentmappings)
                {
                    if (cpbs.DistricDynamicID != null && cpbs.ListTypeID.Equals((int)EnumNameCheck.ProcessEquipmentList))
                        dicMapping["ProcessEquipment"] = true;

                    if (cpbs.DistricDynamicID != null && cpbs.ListTypeID.Equals((int)EnumNameCheck.LineList))
                        dicMapping["LineList"] = true;

                    //if (!lstName.Contains(cpbs.Name))
                    //    lstName.Add(cpbs.Name);
                }

                int iCount = 0;

                foreach (KeyValuePair<string, bool> kv in dicMapping)
                {
                    if (kv.Value)
                        iCount++;
                }

                //if (mProjectSettingMaxCount == iCount)
                //{
                //    node.Image = Resources.smallGreen;
                //}

                bool bfirst = true;
                foreach (string name in lstName)
                {
                    if (bfirst)
                    {
                        sName = name;
                        bfirst = false;
                    }
                    else
                        sName += " / " + name;
                }

                node.Text = string.Format("{0} - [{1}]", sNodeText, sName);

            }
            else
            {
                node.Text = sNodeText;
            }

            dicMapping.Clear();
        }

        #endregion

        #region Event
        private void radButtonAttributeAddAll_Click(object sender, EventArgs e)
        {
            AddAllItem(fpSpreadNotAvailableUnit, fpSpreadAvailableUnit);
        }

        private void radButtonAttributeAdd_Click(object sender, EventArgs e)
        {
            AddItem(fpSpreadNotAvailableUnit, fpSpreadAvailableUnit);
        }

        private void radButtonAttributeRemove_Click(object sender, EventArgs e)
        {
            DeleteItem(fpSpreadNotAvailableUnit, fpSpreadAvailableUnit);
        }

        private void radButtonAttributeRemoveAll_Click(object sender, EventArgs e)
        {
            DeleteAllItem(fpSpreadNotAvailableUnit, fpSpreadAvailableUnit);
        }
        private void fpSpreadDocNo_LeaveCell(object sender, LeaveCellEventArgs e)
        {
            if (this.fpSpreadAvailableUnit.ActiveSheet.Rows.Count > 0)
            {
                this.SelectRow = e.Row;
                this.SaveData();
                SpreadUtil.InitSpread(this.fpSpreadAvailableUnit, mColumnWidth_Menu, mColumnName_Menu, mColumnLock_Menu, null, null, null, null, null, Color.LightGray, true, true);
                this.SelectRow = e.NewRow;
                ClickRoleRow(e.NewRow);
            }
            else
            {
                this.SelectRow = e.NewRow;
                ClickRoleRow(e.NewRow);
            }
        }

        private void ClickRoleRow(int iRow)
        {
            if (radDropDownListProject.SelectedItem != null && radDropDownListProject.SelectedItem.Value != null)
            {
                var project = (EntityEDB_Project)this.radDropDownListProject.SelectedItem.DataBoundItem;
                var ListTypeID = int.Parse(this.radDropDownListListType.SelectedValue.ToString());

                SetSpread_Unit(project.ProjectID, ListTypeID);
            }
        }
        #endregion
        private void AddAllItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            if (FpSpread_NotUse.ActiveSheet != null)
            {
                int iRowCount = FpSpread_NotUse.ActiveSheet.Rows.Count;

                for (int i = 0; i < iRowCount; i++)
                {
                    FpSpread_Use.ActiveSheet.Rows.Count++;

                    for (int j = 0; j < (int)enColumn_Menu.Last; j++)
                    {
                        FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Text
                            = FpSpread_NotUse.ActiveSheet.Cells[i, j].Text;
                    }
                    FpSpread_NotUse.ActiveSheet.Rows[i].Tag = "Delete";
                }

                for (int i = 0; i < FpSpread_NotUse.ActiveSheet.Rows.Count; i++)
                {
                    if (i < 0)
                        break;

                    if (FpSpread_NotUse.ActiveSheet.Rows[i].Tag != null && FpSpread_NotUse.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                    {
                        FpSpread_NotUse.ActiveSheet.Rows[i].Remove();
                        i--;
                    }
                }
            }
        }

        private void AddItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            CellRange[] cr = FpSpread_NotUse.ActiveSheet.GetSelections();

            if (cr.Count() > 0)
            {
                for (int i = cr[0].Row; i < cr[0].Row + cr[0].RowCount; i++)
                {
                    FpSpread_Use.ActiveSheet.Rows.Count++;

                    for (int j = 0; j < (int)enColumn_Menu.Last; j++)
                    {
                        FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Text
                            = FpSpread_NotUse.ActiveSheet.Cells[i, j].Text;
                    }
                    FpSpread_NotUse.ActiveSheet.Rows[i].Tag = "Delete";
                }
            }
            else if (FpSpread_NotUse.ActiveSheet.ActiveCell != null)
            {
                FpSpread_Use.ActiveSheet.Rows.Count++;
                for (int j = 0; j < (int)enColumn_Menu.Last; j++)
                {
                    FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.Rows.Count - 1, j].Text
                        = FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.ActiveCell.Row.Index, j].Text;
                }
                FpSpread_NotUse.ActiveSheet.ActiveCell.Row.Tag = "Delete";
            }

            for (int i = 0; i < FpSpread_NotUse.ActiveSheet.Rows.Count; i++)
            {
                if (i < 0)
                    break;

                if (FpSpread_NotUse.ActiveSheet.Rows[i].Tag != null && FpSpread_NotUse.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                {
                    FpSpread_NotUse.ActiveSheet.Rows[i].Remove();
                    i--;
                }
            }

            FpSpread_Use.ActiveSheet.SetActiveCell(FpSpread_Use.ActiveSheet.Rows.Count - 1, (int)enColumn_Menu.UnitName, true);
            FpSpread_Use.ShowRow(0, FpSpread_Use.ActiveSheet.Rows.Count, VerticalPosition.Bottom);

        }

        private void DeleteItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            CellRange[] cr = FpSpread_Use.ActiveSheet.GetSelections();

            if (cr.Count() > 0)
            {
                for (int i = cr[0].Row; i < cr[0].Row + cr[0].RowCount; i++)
                {
                    FpSpread_NotUse.ActiveSheet.Rows.Count++;

                    for (int j = 0; j < (int)enColumn_Menu.Last; j++)
                    {
                        FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.Rows.Count - 1, j].Text
                            = FpSpread_Use.ActiveSheet.Cells[i, j].Text;
                    }
                    FpSpread_Use.ActiveSheet.Rows[i].Tag = "Delete";
                }
            }
            else if (FpSpread_Use.ActiveSheet.ActiveCell != null)
            {
                FpSpread_NotUse.ActiveSheet.Rows.Count++;
                for (int j = 0; j < (int)enColumn_Menu.Last; j++)
                {
                    FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.Rows.Count - 1, j].Text
                        = FpSpread_Use.ActiveSheet.Cells[FpSpread_Use.ActiveSheet.ActiveCell.Row.Index, j].Text;
                }
                FpSpread_Use.ActiveSheet.ActiveCell.Row.Tag = "Delete";
            }

            for (int i = 0; i < FpSpread_Use.ActiveSheet.Rows.Count; i++)
            {
                if (i < 0)
                    break;

                if (FpSpread_Use.ActiveSheet.Rows[i].Tag != null && FpSpread_Use.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                {
                    string sAttributeName = FpSpread_Use.ActiveSheet.Cells[i, (int)enColumn_Menu.UnitName].Text;

                    FpSpread_Use.ActiveSheet.Rows[i].Remove();
                    i--;
                }
            }

            FpSpread_NotUse.ActiveSheet.SetActiveCell(FpSpread_NotUse.ActiveSheet.Rows.Count - 1, (int)enColumn_Menu.UnitName, true);
            FpSpread_NotUse.ShowRow(0, FpSpread_NotUse.ActiveSheet.Rows.Count, VerticalPosition.Bottom);
        }

        private void DeleteAllItem(FpSpread FpSpread_NotUse, FpSpread FpSpread_Use)
        {
            if (FpSpread_Use.ActiveSheet != null)
            {
                int iRowCount = FpSpread_Use.ActiveSheet.Rows.Count;

                for (int i = 0; i < iRowCount; i++)
                {
                    FpSpread_NotUse.ActiveSheet.Rows.Count++;

                    for (int j = 0; j < (int)enColumn_Menu.Last; j++)
                    {
                        FpSpread_NotUse.ActiveSheet.Cells[FpSpread_NotUse.ActiveSheet.Rows.Count - 1, j].Text
                            = FpSpread_Use.ActiveSheet.Cells[i, j].Text;
                    }
                    FpSpread_Use.ActiveSheet.Rows[i].Tag = "Delete";
                }

                for (int i = 0; i < FpSpread_Use.ActiveSheet.Rows.Count; i++)
                {
                    if (i < 0)
                        break;

                    if (FpSpread_Use.ActiveSheet.Rows[i].Tag != null && FpSpread_Use.ActiveSheet.Rows[i].Tag.ToString() == "Delete")
                    {
                        FpSpread_Use.ActiveSheet.Rows[i].Remove();
                        i--;
                    }
                }
            }
        }

        public override void SaveData()
        {
            try
            {
                List<Common_DocumentNoMapping> DocumentMppings = new List<Common_DocumentNoMapping>();
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    if (this.radDropDownListProject.SelectedItem != null && this.radDropDownListProject.SelectedItem.Value != null)
                    {
                        for (int i = 0; i < this.fpSpreadAvailableUnit.ActiveSheet.Rows.Count; i++)
                        {
                            Common_DocumentNoMapping DocumentMapping = new Common_DocumentNoMapping();
                            DocumentMapping.DocMappingID = 0;
                            DocumentMapping.ProjectMasterID = (long)this.radDropDownListProject.SelectedValue;
                            DocumentMapping.ListTypeID = (int)this.radDropDownListListType.SelectedValue;
                            DocumentMapping.DistricDynamicID = int.Parse(this.fpSpreadAvailableUnit.ActiveSheet.Cells[i, (int)enColumn_Menu.DistricDynamicID].Text);
                            DocumentMapping.DistricDynamicNameView = this.fpSpreadAvailableUnit.ActiveSheet.Cells[i, (int)enColumn_Menu.UnitName].Text;
                            DocumentMapping.DocumentNo = this.fpSpreadDocNo.ActiveSheet.Cells[SelectRow, (int)enColumn.DocNo].Text;

                            DocumentMppings.Add(DocumentMapping);
                        }
                        proxy.CreateDocumentNoMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, DocumentMppings,
                            (long)this.radDropDownListProject.SelectedValue, (int)this.radDropDownListListType.SelectedValue, this.fpSpreadDocNo.ActiveSheet.Cells[SelectRow, (int)enColumn.DocNo].Text);
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
    }
}
