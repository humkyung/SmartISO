using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL.Enums;
using EDCS.WF.BSL.Helpers;
using EDCS.WF.BSL.Method;
using EDCS.WF.BSL.Variable;
using EDCS.WF.DSL;
using Telerik.WinControls;
using Telerik.WinControls.UI;
using Telerik.WinControls.UI.Data;

namespace Configuration
{
    public partial class UserControlEDBTagNamingRule : UserControlBase
    {
        private string[] _delimiterArray = { "", "-" };
        private string[] _typeArray = { "Property", "Text" };
        private List<EntityEDB_ColumnDynamicDefault> _columnDynamics;// = new List<EntityColumnDynamic>();

        public UserControlEDBTagNamingRule()
        {
            try
            {
                this.InitializeComponent();
                this.Dock = DockStyle.Fill;

                // General
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
                    this.radGridViewTagNamingRule.CellEditorInitialized += radGridViewTagNamingRule_CellEditorInitialized;
                    this.radGridViewTagNamingRule.CellValueChanged += radGridViewTagNamingRule_CellValueChanged;
                    this.radGridViewTagNamingRule.PreviewKeyDown += radGridViewTagNamingRule_PreviewKeyDown;

                    this.radDropDownListListType.ValueMember = "ListTypeID";
                    this.radDropDownListListType.DisplayMember = "ComplexNameView";
                    this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                    this.radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;

                    this.radDropDownListListType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListListType_SelectedIndexChanged);
                    this.radDropDownListListType.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListListType_SelectedIndexChanging);

                    var projects = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true).ToList();
                    projects.ConvertAll(ca => ca.ComplexNameView = ca.ComplexNameView + " - " + ca.Type);
                    this.radDropDownListProject.Items.Clear();
                    this.radDropDownListProject.ValueMember = "ProjectID";
                    this.radDropDownListProject.DisplayMember = "ComplexNameView";
                    this.radDropDownListProject.DataSource = projects;
                    //this.radDropDownListProject.SelectedValue = null;
                    this.radDropDownListProject.SelectedValue = this._entityEDBInformation.Project.ProjectID;
                    this.radDropDownListProject.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListProject_SelectedIndexChanged);
                    this.radDropDownListProject.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListProject_SelectedIndexChanging);

                    this.MethodBindingRadGridView();
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

        void radDropDownListProject_SelectedIndexChanging(object sender, PositionChangingCancelEventArgs e)
        {
            var newProject = (EntityEDB_Project)this.radDropDownListProject.Items[e.Position].DataBoundItem;
            var oldProject = (EntityEDB_Project)this.radDropDownListProject.SelectedItem.DataBoundItem;
            if (!newProject.ListTypeFlag.Contains(this._entityEDBInformation.ListType.ListTypeID))
            {
                //e.Cancel = true;
            }
        }

        void radDropDownListListType_SelectedIndexChanging(object sender, PositionChangingCancelEventArgs e)
        {
            var newListType = (EntityEDB_ListType)this.radDropDownListListType.Items[e.Position].DataBoundItem;
            var oldListType = (EntityEDB_ListType)this.radDropDownListListType.SelectedItem.DataBoundItem;
            if (!this._entityEDBInformation.Project.ListTypeFlag.Contains(newListType.ListTypeID))
            {
                e.Cancel = true;
            }
        }

        void radGridViewTagNamingRule_CellEditorInitialized(object sender, GridViewCellEventArgs e)
        {
            if (e.Column != null && e.Column.Name == "Column")
            {
                string rType = this.radGridViewTagNamingRule.Rows[e.RowIndex].Cells["Type"].Value as string;
                string rColumn = this.radGridViewTagNamingRule.Rows[e.RowIndex].Cells["Column"].Value as string;
                if (rType == "Text") // delimiter
                {
                    RadDropDownListEditor editor = this.radGridViewTagNamingRule.ActiveEditor as RadDropDownListEditor;
                    if (editor != null)
                    {
                        ((RadDropDownListEditorElement)((RadDropDownListEditor)this.radGridViewTagNamingRule.ActiveEditor).EditorElement).DataSource = _delimiterArray;
                        ((RadDropDownListEditorElement)((RadDropDownListEditor)this.radGridViewTagNamingRule.ActiveEditor).EditorElement).ValueMember = "LabelView";
                        ((RadDropDownListEditorElement)((RadDropDownListEditor)this.radGridViewTagNamingRule.ActiveEditor).EditorElement).SelectedValue = rColumn;
                    }
                }
                else if (rType == "Property") // Property
                {
                    RadDropDownListEditor editor = this.radGridViewTagNamingRule.ActiveEditor as RadDropDownListEditor;
                    if (editor != null)
                    {
                        ((RadDropDownListEditorElement)((RadDropDownListEditor)this.radGridViewTagNamingRule.ActiveEditor).EditorElement).Items.Clear();
                        ((RadDropDownListEditorElement)((RadDropDownListEditor)this.radGridViewTagNamingRule.ActiveEditor).EditorElement).DataSource = this._columnDynamics.Where(w => w.Label != SharedVariable.ITEM_TAG).ToList();
                        ((RadDropDownListEditorElement)((RadDropDownListEditor)this.radGridViewTagNamingRule.ActiveEditor).EditorElement).DisplayMember = "LabelView";
                        ((RadDropDownListEditorElement)((RadDropDownListEditor)this.radGridViewTagNamingRule.ActiveEditor).EditorElement).ValueMember = "LabelView";
                        ((RadDropDownListEditorElement)((RadDropDownListEditor)this.radGridViewTagNamingRule.ActiveEditor).EditorElement).SelectedValue = rColumn;
                    }
                }
            }
        }


        private void radGridViewTagNamingRule_CellValueChanged(object sender, GridViewCellEventArgs e)
        {
            if (e.Column != null && e.Column.Name == "Column")
            {
                string rType = this.radGridViewTagNamingRule.Rows[e.RowIndex].Cells["Type"].Value as string;
                if (rType == "Property" && e.Value != null) // PreView
                {
                    var column = _columnDynamics.Where(w => w.LabelView == e.Value.ToString()).SingleOrDefault();
                    if (column != null)
                    {
                        this.radGridViewTagNamingRule.Rows[e.RowIndex].Cells["Label"].Value = column.Label;

                        this.radLabelView.Text = "";
                        for (int nRow = 0; nRow < this.radGridViewTagNamingRule.RowCount; nRow++)
                        {
                            if (this.radGridViewTagNamingRule.Rows[nRow].Cells["Column"].Value != null && this.radGridViewTagNamingRule.Rows[nRow].Cells["Type"].Value != null)
                            {
                                if (this.radGridViewTagNamingRule.Rows[nRow].Cells["Type"].Value.ToString() == "Text")
                                {
                                    this.radLabelView.Text += (" " + this.radGridViewTagNamingRule.Rows[nRow].Cells["Column"].Value.ToString() + " ");
                                }
                                else
                                {
                                    this.radLabelView.Text += (" [" + this.radGridViewTagNamingRule.Rows[nRow].Cells["Column"].Value.ToString() + "] ");
                                }
                            }
                        }

                        // Label을 다시 입력
                        this.radGridViewTagNamingRule.Rows[e.RowIndex].Cells["Label"].Value = column.Label;
                    }
                }
            }
            else if (e.Column != null && e.Column.Name == "Type")
            {
                // Column combobox, Label 초기화
                this.radGridViewTagNamingRule.Rows[e.RowIndex].Cells["Column"].Value = null;
                this.radGridViewTagNamingRule.Rows[e.RowIndex].Cells["Label"].Value = null;
            }
        }


        private void radGridViewTagNamingRule_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyValue.Equals(46)) // DELETE
            {
                if (this.radGridViewTagNamingRule.SelectedRows.Count > 0)
                {
                    for (int i = 0; i < this.radGridViewTagNamingRule.SelectedRows[0].Cells.Count; i++)
                    {
                        this.radGridViewTagNamingRule.SelectedRows[0].Cells[i].Value = string.Empty;
                    }

                    this.radLabelView.Text = "";
                    for (int nRow = 0; nRow < this.radGridViewTagNamingRule.RowCount; nRow++)
                    {
                        if (this.radGridViewTagNamingRule.Rows[nRow].Cells["Column"].Value != null && this.radGridViewTagNamingRule.Rows[nRow].Cells["Type"].Value != null)
                        {
                            if (this.radGridViewTagNamingRule.Rows[nRow].Cells["Type"].Value.ToString() == "Text")
                            {
                                this.radLabelView.Text += (" " + this.radGridViewTagNamingRule.Rows[nRow].Cells["Column"].Value.ToString() + " ");
                            }
                            else
                            {
                                this.radLabelView.Text += (" [" + this.radGridViewTagNamingRule.Rows[nRow].Cells["Column"].Value.ToString() + "] ");
                            }
                        }
                    }

                }

            }
        }

        void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            this._entityEDBInformation.Project = (EntityEDB_Project)this.radDropDownListProject.SelectedItem.DataBoundItem;
            this.MethodBindingRadGridView();
        }
        void radDropDownListListType_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            this._entityEDBInformation.ListType = (EntityEDB_ListType)this.radDropDownListListType.SelectedItem.DataBoundItem;
            this.MethodBindingRadGridView();
        }

        public override void SaveData()
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    if (this.radGridViewTagNamingRule.Rows.Where(w => w.Cells["Type"].Value != null && w.Cells["Type"].Value.ToString() == "Property").Count() !=
                            this.radGridViewTagNamingRule.Rows.Where(w => w.Cells["Type"].Value != null && w.Cells["Type"].Value.ToString() == "Property").Select(s => s.Cells["Label"].Value).Distinct().Count())
                    {
                        new CustomRadMessageBoxShowAndTheme("It is duplicated.").RadMessageBoxShow();
                        return;
                    }

                    //.Cells["Type"]
                    // Tag Naming Rule Save
                    var tagNamingRuleList = new List<EDB_TagNamingRuleDynamic>();
                    for (int nRow = 0; nRow < this.radGridViewTagNamingRule.RowCount; nRow++)
                    {
                        if (this.radGridViewTagNamingRule.Rows[nRow].Cells["Type"].Value == null || this.radGridViewTagNamingRule.Rows[nRow].Cells["Column"].Value == null)
                        {
                            continue;
                        }

                        if (this.radGridViewTagNamingRule.Rows[nRow].Cells["Type"].Value.ToString().Equals("Text"))
                        {
                            this.radGridViewTagNamingRule.Rows[nRow].Cells["Label"].Value = null;
                        }

                        tagNamingRuleList.Add(
                            new EDB_TagNamingRuleDynamic()
                            {
                                Label = this.radGridViewTagNamingRule.Rows[nRow].Cells["Label"].Value as string,
                                Type = this.radGridViewTagNamingRule.Rows[nRow].Cells["Type"].Value as string,
                                ColumnName = this.radGridViewTagNamingRule.Rows[nRow].Cells["Column"].Value as string,
                                ModifiedDate = DateTime.Now,
                                ProjectMasterID = this._entityEDBInformation.Project.ProjectMasterID,
                                ListTypeID = this._entityEDBInformation.ListType.ListTypeID,


                            });
                    }

                    //Tag Naming Rule  끝에 - (Text)가 있으면 제거
                    for (int i = tagNamingRuleList.Count; i > 0; --i)
                    {
                        if (string.IsNullOrEmpty(tagNamingRuleList[i - 1].Label)) tagNamingRuleList.RemoveAt(i - 1);
                        else break;
                    }

                    tagNamingRuleList.RemoveAll(ra => string.IsNullOrWhiteSpace(ra.Type) && string.IsNullOrWhiteSpace(ra.Label));
                    // DB에 저장
                    proxy.CreateTagNamingRuleDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, tagNamingRuleList, this._entityEDBInformation.Project.ProjectMasterID, this._entityEDBInformation.ListType.ListTypeID);

                    var projects = proxy.ReadCaseProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.CaseProjectID, this._entityEDBInformation.ListType.ListTypeID);
                    var caseDynamics = proxy.ReadCaseDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.CaseProjectID, this._entityEDBInformation.ListType.ListTypeID);
                    var ColumnDynamicParents = proxy.ReadColumnDynamicParents(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.CaseProjectID, this._entityEDBInformation.ListType.ListTypeID, p_onlyParent: true);
                    var columnDynamics = proxy.ReadColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.CaseProjectID, this._entityEDBInformation.ListType.ListTypeID);

                    foreach (var project in projects)
                    {
                        // 메인 DB에 적용
                        var rowDynamics = proxy.ReadRowDynamicsAll(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, project.ProjectID, this._entityEDBInformation.ListType.ListTypeID);//, this._entityFrmTagNamingRule.Discipline.DisciplineID);
                        //var cellDynamics = proxy.ReadCellDynamics(false, this._entityFrmTagNamingRule.Project.ProjectID, this._entityFrmTagNamingRule.ListType.ListTypeID).Where(w => w.DisciplineID == this._entityFrmTagNamingRule.Discipline.DisciplineID).ToList();
                        var cellDynamics = proxy.ReadCellDynamicsByDisciplineID(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, project.ProjectID, this._entityEDBInformation.ListType.ListTypeID);//.Where(w => w.DisciplineID == this._entityFrmTagNamingRule.Discipline.DisciplineID).ToList();
                        var sheetDynamics = proxy.ReadSheetDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, project.ProjectID, this._entityEDBInformation.ListType.ListTypeID);

                        var newCellDynamicsForInsertList = new List<EDB_CellDynamic>();
                        var newCellDynamicsForUpdateList = new List<EDB_CellDynamic>();

                        var disciplineList = cellDynamics.Select(s => s.DisciplineID).Distinct().ToList();

                        int nSheetIndex = 0; // 시트마다 column이 같으므로 첫번째 sheet만 변경 저장하기위해
                        foreach (var sheetDynamic in sheetDynamics)
                        {
                            var cellDynamicsBySheet = cellDynamics.Where(w => w.SheetDynamicID == sheetDynamic.SheetDynamicID).ToList();
                            foreach (var caseDynamic in caseDynamics)
                            {
                                var columnDynamicParentByCase = ColumnDynamicParents.Where(w => w.CaseDynamicID == caseDynamic.CaseDynamicID).SingleOrDefault();
                                var columnDynamicsByCase = columnDynamics.Where(w => w.ColumnDynamicParentID == columnDynamicParentByCase.ColumnDynamicParentID).ToList();

                                // Tag Naming Rule에 따라 ITEMNO column Columnindex를 변경
                                #region  - Tag Naming Rule에 따라 ITEMNO column Columnindex를 변경 - 물어볼것.
                                bool IsChangedColumnIndex = false;

                                if (nSheetIndex == 0)
                                {
                                    var indexChangeColumnDynamic = new List<EntityEDB_ColumnDynamic>();
                                    int nTempColumnIndex = -1;
                                    var tagNamingRuleOnlyColumList = new List<EDB_TagNamingRuleDynamic>(tagNamingRuleList.Where(w => !string.IsNullOrWhiteSpace(w.Label)));
                                    for (int j = 0; j < tagNamingRuleOnlyColumList.Count; j++)
                                    {
                                        for (int i = 0; i < tagNamingRuleOnlyColumList.Count - 1; i++)
                                        {
                                            var column = columnDynamicsByCase.Where(w => w.Label == tagNamingRuleOnlyColumList[i].Label).SingleOrDefault();
                                            var nextcolumn = columnDynamicsByCase.Where(w => w.Label == tagNamingRuleOnlyColumList[i + 1].Label).SingleOrDefault();
                                            if (column != null)
                                            {
                                                if (column.ColumnIndex > nextcolumn.ColumnIndex)
                                                {
                                                    nTempColumnIndex = column.ColumnIndex;
                                                    column.ColumnIndex = nextcolumn.ColumnIndex;
                                                    nextcolumn.ColumnIndex = nTempColumnIndex;

                                                    IsChangedColumnIndex = true; // ColumnIndex가 변경됐을때만 저장하기 위해
                                                }
                                            }
                                        }
                                    }
                                    IsChangedColumnIndex = false;
                                    // ColumnDynamic tag rule에 따라 columnindex 변경되면 저장
                                    if (IsChangedColumnIndex)
                                    {
                                        var newColumnDynamics = new List<EDB_ColumnDynamic>();
                                        foreach (var columnDynamic in columnDynamicsByCase)
                                        {
                                            EDB_ColumnDynamic newColumnDynamic = new EDB_ColumnDynamic();
                                            newColumnDynamic.ColumnDynamicID = columnDynamic.ColumnDynamicID;
                                            newColumnDynamic.ColumnIndex = columnDynamic.ColumnIndex;
                                            newColumnDynamic.CommonUnitID = columnDynamic.CommonUnitID;
                                            newColumnDynamic.Orders = columnDynamic.Orders;
                                            newColumnDynamic.Label = columnDynamic.Label;
                                            newColumnDynamic.LabelView = columnDynamic.LabelView;
                                            newColumnDynamic.CellType = columnDynamic.CellType;
                                            newColumnDynamic.Locked = columnDynamic.Locked;
                                            newColumnDynamic.Width = columnDynamic.Width;
                                            newColumnDynamic.ColumnDynamicParentID = columnDynamic.ColumnDynamicParentID;
                                            newColumnDynamics.Add(newColumnDynamic);

                                        } // end foreach

                                        proxy.CreateColumnDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newColumnDynamics, this._entityEDBInformation.Project.ProjectID, this._entityEDBInformation.ListType.ListTypeID, columnDynamicParentByCase.CaseDynamicID, null);
                                    }

                                }
                                #endregion //----------------------------------------------------

                                // 부서별
                                var cellDynamicsBySheetByCase = cellDynamicsBySheet.Where(w => w.CaseDynamicID == caseDynamic.CaseDynamicID).ToList();

                                foreach (var discipline in disciplineList)
                                {
                                    var cellDynamicsBySheetByCaseByDiscipline = cellDynamicsBySheetByCase.Where(w => w.DisciplineID == discipline).ToList();
                                    foreach (var cellbyRow in cellDynamicsBySheetByCaseByDiscipline.GroupBy(gb => gb.RowIndex))
                                    {

                                        string rItemTag = string.Empty;
                                        for (int i = 0; i < tagNamingRuleList.Count; i++)
                                        {
                                            if (!string.IsNullOrWhiteSpace(tagNamingRuleList[i].Label)) // Naming rule
                                            {
                                                var ColumnDynamic = columnDynamicsByCase.Where(w => w.Label == tagNamingRuleList[i].Label).SingleOrDefault();
                                                if (ColumnDynamic != null)
                                                {
                                                    var cell = cellbyRow.Where(w => w.ColumnDynamicID == ColumnDynamic.ColumnDynamicID).SingleOrDefault();
                                                    if (cell != null)
                                                    {
                                                        rItemTag += cell.Value;
                                                    }
                                                }
                                            }
                                            else // delimiter
                                            {
                                                rItemTag += tagNamingRuleList[i].ColumnName;
                                            }
                                        }

                                        rItemTag = SharedMethod.MethodRemoveTagNamingRuleHyphen(rItemTag);

                                        // Tag Naming Rule에 의해 만들진 Tag를 해당 Cell에 넣어준다.
                                        var column = columnDynamicsByCase.Where(w => w.Label == SharedVariable.ITEM_TAG).SingleOrDefault();
                                        if (column != null)
                                        {
                                            var cell = cellbyRow.Where(w => w.ColumnDynamicID == column.ColumnDynamicID).SingleOrDefault();
                                            if (cell != null) // 이전 값이 있으면 (UPDATE)
                                            {

                                                newCellDynamicsForUpdateList.Add(
                                                new EDB_CellDynamic()
                                                {
                                                    SheetDynamicID = cell.SheetDynamicID,
                                                    CaseDynamicID = cell.CaseDynamicID,
                                                    ColumnDynamicID = cell.ColumnDynamicID,
                                                    DisciplineID = discipline,
                                                    OriginDisciplineID = cell.OriginDisciplineID,
                                                    FirstRowDynamicID = cell.FirstRowDynamicID,
                                                    ModifiedDate = DateTime.Now,
                                                    EmployeeID = this._entityEDBInformation.Employee.EmployeeID,
                                                    RowIndex = cell.RowIndex,
                                                    Value = rItemTag,
                                                    Formula = cell.Formula,
                                                    Note = cell.Note,
                                                    FontName = cell.FontName,
                                                    FontSize = cell.FontSize,
                                                    FontBold = cell.FontBold,
                                                    FontItalic = cell.FontItalic,
                                                    FontUnderline = cell.FontUnderline,
                                                    FontStrikeout = cell.FontStrikeout,
                                                    BackColorRed = cell.BackColorRed,
                                                    BackColorGreen = cell.BackColorGreen,
                                                    BackColorBlue = cell.BackColorBlue,
                                                    ForeColorRed = cell.ForeColorRed,
                                                    ForeColorGreen = cell.ForeColorGreen,
                                                    ForeColorBlue = cell.ForeColorBlue,
                                                    BorderLeft = cell.BorderLeft,
                                                    BorderTop = cell.BorderTop,
                                                    BorderRight = cell.BorderRight,
                                                    BorderBottom = cell.BorderBottom,
                                                    VerticalAlignment = cell.VerticalAlignment,
                                                    HorizontalAlignment = cell.HorizontalAlignment,
                                                    TextIndent = cell.TextIndent,
                                                    WordWrap = cell.WordWrap,
                                                    IsKeyin = cell.IsKeyin,

                                                });
                                            }
                                            else // Item Tag가 없었으면 (INSERT)
                                            {
                                                newCellDynamicsForInsertList.Add(
                                                new EDB_CellDynamic()
                                                {
                                                    SheetDynamicID = sheetDynamic.SheetDynamicID,
                                                    CaseDynamicID = caseDynamic.CaseDynamicID,
                                                    ColumnDynamicID = column.ColumnDynamicID,
                                                    DisciplineID = discipline,
                                                    OriginDisciplineID = this._entityEDBInformation.ListType.DisciplineID,
                                                    VisibleFlag = true,
                                                    DeleteFlag = false,
                                                    ModifiedDate = DateTime.Now,
                                                    EmployeeID = this._entityEDBInformation.Employee.EmployeeID,
                                                    RowIndex = cellbyRow.FirstOrDefault().RowIndex,
                                                    FirstRowDynamicID = cellbyRow.FirstOrDefault().FirstRowDynamicID,
                                                    Value = rItemTag,
                                                });
                                            }
                                        }
                                    }
                                }
                            }
                            nSheetIndex++;
                        }

                        var saveHistories = new List<EDB_SaveHistory>();
                        var newCellDynamicAll = newCellDynamicsForUpdateList.Concat(newCellDynamicsForInsertList).ToList();
                        foreach (var cellDynamicGroup in newCellDynamicAll.GroupBy(gb => gb.DisciplineID))
                        {
                            EDB_SaveHistory saveHistory = new EDB_SaveHistory();
                            var sheetDynamicIDstring = string.Empty;
                            var caseDynamicIDstring = string.Empty;

                            cellDynamicGroup.Select(s => s.SheetDynamicID).ToList().ForEach(fe => sheetDynamicIDstring += fe.ToString() + "/");
                            sheetDynamicIDstring.Remove(sheetDynamicIDstring.Length - 1);
                            cellDynamicGroup.Select(s => s.CaseDynamicID).ToList().ForEach(fe => caseDynamicIDstring += fe.ToString() + "/");
                            caseDynamicIDstring.Remove(caseDynamicIDstring.Length - 1);
                            saveHistory.DisciplineID = cellDynamicGroup.Key;
                            saveHistory.EmployeeID = this._entityEDBInformation.Employee.EmployeeID;
                            saveHistory.ListTypeID = this._entityEDBInformation.ListType.ListTypeID;
                            saveHistory.ProjectID = this._entityEDBInformation.Project.ProjectID;
                            saveHistory.SheetDynamicID = sheetDynamicIDstring;
                            saveHistory.CaseDynamicID = caseDynamicIDstring;
                            saveHistory.Description = "Tag Naming Rule Setting";
                            proxy.CreateSaveHistory(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref  saveHistory);
                            saveHistories.Add(saveHistory);
                        }

                        foreach (var saveHistory in saveHistories)
                        {
                            newCellDynamicAll.Where(w => w.DisciplineID == saveHistory.DisciplineID).ToList().ConvertAll(ca => ca.SaveHistoryID = saveHistory.SaveHistoryID);
                        }

                        // Update 항목 저장
                        if (newCellDynamicsForUpdateList.Count > 0)
                        {
                            proxy.CreateCellDynamicTagNamingRule(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newCellDynamicsForUpdateList, newCellDynamicsForUpdateList.Select(s => s.ColumnDynamicID).Distinct().ToList(), true);
                        }

                        // Insert 항목 저장
                        if (newCellDynamicsForInsertList.Count > 0)
                        {
                            proxy.CreateCellDynamicTagNamingRule(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newCellDynamicsForInsertList, newCellDynamicsForInsertList.Select(s => s.ColumnDynamicID).Distinct().ToList(), false);
                        }
                    }
                    this.Cursor = Cursors.Default;
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
        private void MethodDesignForm()
        {
            // radGridView1
            this.radGridViewTagNamingRule.AllowAutoSizeColumns = false;
            this.radGridViewTagNamingRule.AllowColumnChooser = false;
            this.radGridViewTagNamingRule.AllowColumnHeaderContextMenu = false;
            this.radGridViewTagNamingRule.AllowColumnReorder = false;
            this.radGridViewTagNamingRule.AllowMultiColumnSorting = false;
            this.radGridViewTagNamingRule.AllowRowResize = false;
            this.radGridViewTagNamingRule.AutoGenerateColumns = false;
            this.radGridViewTagNamingRule.AutoSizeColumnsMode = GridViewAutoSizeColumnsMode.Fill;
            this.radGridViewTagNamingRule.EnableHotTracking = false;
            this.radGridViewTagNamingRule.ShowCellErrors = false;
            this.radGridViewTagNamingRule.ShowFilteringRow = false;
            this.radGridViewTagNamingRule.ShowNoDataText = false;
            this.radGridViewTagNamingRule.ShowRowErrors = false;
            this.radGridViewTagNamingRule.TableElement.TableHeaderHeight = 25;
            this.radGridViewTagNamingRule.TableElement.RowHeight = 30;
            //
            this.radGridViewTagNamingRule.MultiSelect = false;
            this.radGridViewTagNamingRule.EnableGrouping = false;
            this.radGridViewTagNamingRule.AllowAddNewRow = false;
            this.radGridViewTagNamingRule.AllowDeleteRow = true;
            this.radGridViewTagNamingRule.AllowRowReorder = false;
            this.radGridViewTagNamingRule.AllowAddNewRow = false;

            this.radGridViewTagNamingRule.RowCount = 11;
            this.radGridViewTagNamingRule.AllowCellContextMenu = false;
            this.radGridViewTagNamingRule.ColumnChooserSortOrder = RadSortOrder.None;
        }

        private void MethodBindingRadGridView()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                //this.radGridViewTagNamingRule.Rows.Clear();
                this.radGridViewTagNamingRule.Columns.Clear();

                this._columnDynamics = proxy.ReadCommonColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.ListType.ListTypeID)
                        .Select(s => new EntityEDB_ColumnDynamicDefault()
                        {
                            Label = s.Name,
                            LabelView = s.NameView,
                        }).ToList(); ;

                var ItemNoColumnList = new List<string>();
                ItemNoColumnList.Add("-");
                ItemNoColumnList.AddRange(this._columnDynamics.Select(s => s.LabelView).ToList());
                this.radGridViewTagNamingRule.Columns.Add(new GridViewCheckBoxColumn()
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
                this.radGridViewTagNamingRule.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "TagNamingRuleID",
                    IsVisible = false,
                    ReadOnly = true,
                });
                this.radGridViewTagNamingRule.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "Label",
                    IsVisible = false,
                    ReadOnly = true,
                });
                this.radGridViewTagNamingRule.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "Type",
                    HeaderText = "Type",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    DataSource = _typeArray,
                    Width = 30,
                    DropDownStyle = RadDropDownStyle.DropDown,
                    AllowSort = false,
                });
                this.radGridViewTagNamingRule.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "Column",
                    HeaderText = "Column",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    DataSource = ItemNoColumnList,
                    DropDownStyle = RadDropDownStyle.DropDown,
                    AllowSort = false,


                });
                var TagNameingRuleDynamics = proxy.ReadTagNamingRuleDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.Project.ProjectMasterID, this._entityEDBInformation.ListType.ListTypeID);

                this.radLabelView.Text = "";
                this.radGridViewTagNamingRule.RowCount = TagNameingRuleDynamics.Count;
                int nRow = 0;

                this.radGridViewTagNamingRule.CellValueChanged -= radGridViewTagNamingRule_CellValueChanged;
                foreach (var tagNamingRuleDynamic in TagNameingRuleDynamics)
                {
                    this.radGridViewTagNamingRule.Rows[nRow].Cells["TagNamingRuleID"].Value = tagNamingRuleDynamic.TagNamingRuleDynamicID;
                    this.radGridViewTagNamingRule.Rows[nRow].Cells["Label"].Value = tagNamingRuleDynamic.Label;
                    this.radGridViewTagNamingRule.Rows[nRow].Cells["Type"].Value = tagNamingRuleDynamic.Type;
                    this.radGridViewTagNamingRule.Rows[nRow].Cells["Column"].Value = tagNamingRuleDynamic.ColumnName;
                    //this.radGridViewTagNamingRule.Rows[nRow].Cells["Length"].Value = tagNamingRuleDynamic.Length;

                    if (this.radGridViewTagNamingRule.Rows[nRow].Cells["Column"].Value != null && this.radGridViewTagNamingRule.Rows[nRow].Cells["Type"].Value != null)
                    {
                        if (this.radGridViewTagNamingRule.Rows[nRow].Cells["Type"].Value.ToString() == "Text")
                        {
                            this.radLabelView.Text += (" " + tagNamingRuleDynamic.ColumnName + " ");
                        }
                        else
                        {
                            this.radLabelView.Text += (" [" + tagNamingRuleDynamic.ColumnName + "] ");
                        }
                    }

                    nRow++;
                }
                this.radGridViewTagNamingRule.RowCount = TagNameingRuleDynamics.Count;
                this.radGridViewTagNamingRule.CellValueChanged += radGridViewTagNamingRule_CellValueChanged;
            }
        }

        private void radButtonUp_Click(object sender, EventArgs e)
        {
            SharedMethod.MethodRadGridViewMoveRow(this.radGridViewTagNamingRule, true);
        }

        private void radButtonDown_Click(object sender, EventArgs e)
        {
            SharedMethod.MethodRadGridViewMoveRow(this.radGridViewTagNamingRule, false);
        }
        private void radButtonInsertRow_Click(object sender, EventArgs e)
        {
            GridViewRowInfo oGridRow = this.radGridViewTagNamingRule.Rows.AddNew();
        }

        private void radButtonRemoveRow_Click(object sender, EventArgs e)
        {
            for (int i = this.radGridViewTagNamingRule.Rows.Count - 1; i > -1; i--)
            {
                if (this.radGridViewTagNamingRule.Rows[i].Cells["DeleteCheck"].Value != null && (bool)this.radGridViewTagNamingRule.Rows[i].Cells["DeleteCheck"].Value == true)
                {
                    this.radGridViewTagNamingRule.Rows.RemoveAt(i);
                }
            }
        }

    }
}
