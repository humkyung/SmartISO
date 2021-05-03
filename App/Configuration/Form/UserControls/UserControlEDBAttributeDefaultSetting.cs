using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using CommonLibrary.CL;
using EDCS.WF.BSL.Converters;
using EDCS.WF.BSL.Entities;
using EDCS.WF.BSL.Enums;
using EDCS.WF.BSL.Helpers;
using EDCS.WF.BSL.Method;
using EDCS.WF.BSL.Variable;
using EDCS.WF.DSL;
using FarPoint.Win;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;
using Telerik.WinControls;
using Telerik.WinControls.UI;

namespace Configuration
{
    public partial class UserControlEDBAttributeDefaultSetting : UserControlBase
    {
        private int _prevCaseDynamicID;
        public List<EntityEDB_CommonColumnDynamic> _commonColumnDynamics;
        public List<EntityEDB_CommonUnit> _commonUnits;
        public List<EntityEnumCellTypeChange> _enumCellTypes;
        public EntityEDB_CaseDynamicDefault CaseDynamic { get; set; }

        private Dictionary<int, EntityEDB_ColumnDynamicParentDefault> _columnDynamicParents;
        private Dictionary<int, EntityEDB_ColumnDynamicParentDefault> _originColumnDynamicParents;

        private Dictionary<int, List<EntityEDB_ColumnDynamicDefault>> _columnDynamics;
        private Dictionary<int, List<EntityEDB_ColumnDynamicDefault>> _originColumnDynamics;

        public int _lastTempColumnDynamicID;
        public int _tempColumnDynamicID;

        public int columnHeaderRowLastIndex;        // Merge Row
        public int columnHeaderRowLabelIndex;   // Label Row
        public int columnHeaderRowUnitIndex;        // Unit  Row

        int ListTypeInstrumentProcessData = 3;

        List<EntityEDB_InstrumentProcessDataOwnership> _ownershipList;

        public UserControlEDBAttributeDefaultSetting()
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
                    this.fpSpread1.CellClick += new CellClickEventHandler(fpSpread1_CellClick);
                    //this.fpSpread1_Sheet1.CellChanged += new SheetViewEventHandler(fpSpread1_Sheet1_CellChanged);
                    this.radMenuItemAddColumn.Click += new EventHandler(radMenuItemAddColumn_Click);
                    this.radMenuItemRemoveColumn.Click += new EventHandler(radMenuItemDeleteColumn_Click);
                    this.radMenuItemRowCount3.Click += new EventHandler(radMenuItemRowCount3_Click);
                    this.radMenuItemRowCount4.Click += new EventHandler(radMenuItemRowCount4_Click);
                    this.radMenuItemMerge.Click += new EventHandler(radMenuItemMerge_Click);
                    this.radMenuItemClear.Click += new EventHandler(radMenuItemClear_Click);

                    this._columnDynamicParents = new Dictionary<int, EntityEDB_ColumnDynamicParentDefault>();
                    this._originColumnDynamicParents = new Dictionary<int, EntityEDB_ColumnDynamicParentDefault>();
                    this._commonUnits = proxy.ReadCommonUnits(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                    this._commonColumnDynamics = proxy.ReadCommonColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.ListType.ListTypeID);
                    this._enumCellTypes = new ConverterEnumCellTypeChange().Convert().OrderBy(ob => ob.Order).ToList();
                    this._ownershipList = proxy.ReadInstrumentProcessDataOwnership(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                    if (_entityEDBInformation.ListType.ListTypeID.Equals(ListTypeInstrumentProcessData))
                    {
                        this.MethodSettingDropDownListVisibleInstrument();
                    }
                    else
                    {
                        this.MethodBindingRadDropDownList();
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

        void radMenuItemClear_Click(object sender, EventArgs e)
        {
            var getSelection = this.fpSpread1.ActiveSheet.GetSelection(0);
            if (getSelection != null)
            {
                var row = getSelection.Row;
                if (row.Equals(-1))
                {
                    row = 0;
                }
                this.fpSpread1.ActiveSheet.RemoveSpanCell(row, getSelection.Column);
                //this.fpSpread1.ActiveSheet.Cells.Get(row, getSelection.Column).ResetText();
            }
        }
        void radMenuItemMerge_Click(object sender, EventArgs e)
        {
            int rowCount = this.columnHeaderRowLastIndex + 1;
            int rowIndexLableView = this.columnHeaderRowLabelIndex;
            int rowIndexUnitView = this.columnHeaderRowUnitIndex;

            var getSelection = this.fpSpread1.ActiveSheet.GetSelection(0);
            if (getSelection != null)
            {
                if (getSelection.RowCount > 1 || getSelection.ColumnCount > 1) // 선택한 셀이 하나 이상일 때
                {
                    bool flagJudgment = true;
                    for (int i = getSelection.Row; i < getSelection.Row + getSelection.RowCount; i++)
                    {
                        if (i.Equals(rowIndexLableView) || i.Equals(rowIndexUnitView))
                        {
                            flagJudgment = false;
                        }
                    } // end for

                    if (flagJudgment)
                    {
                        if (getSelection.Row > -1) // 컬럼헤더를 잡고, 병합을 시도하면 minus row
                        {
                            for (int i = getSelection.Row; i < getSelection.Row + getSelection.RowCount; i++)
                            {
                                //this.fpSpread1.ActiveSheet.AddSpanCell(getSelection.Row, getSelection.Column, getSelection.RowCount, getSelection.ColumnCount);
                                this.fpSpread1.ActiveSheet.AddSpanCell(i, getSelection.Column, 1, getSelection.ColumnCount);
                            }
                        }
                    }
                }
            }
        }

        void radMenuItemRowCount4_Click(object sender, EventArgs e)
        {
            if (this.columnHeaderRowLastIndex != 3)
            {
                this.columnHeaderRowLastIndex = 3;
                this.columnHeaderRowLabelIndex = 2;
                this.columnHeaderRowUnitIndex = 3;

                this.fpSpread1.ActiveSheet.AddRows(0, 1);

                this.fpSpread1.ActiveSheet.RowHeader.Rows[0].Label = "Preview";
                this.fpSpread1.ActiveSheet.RowHeader.Cells[0, 0].RowSpan = columnHeaderRowLastIndex + 1;

                var columnCount = this.fpSpread1.ActiveSheet.ColumnCount;
                var none = new ComplexBorderSide(Color.Gray, 0);
                var thin = new ComplexBorderSide(Color.Gray, 1);
                this.fpSpread1.ActiveSheet.Cells[0, 0, columnHeaderRowLastIndex, columnCount - 1].VerticalAlignment = CellVerticalAlignment.Center;
                this.fpSpread1.ActiveSheet.Cells[0, 0, columnHeaderRowLastIndex, columnCount - 1].HorizontalAlignment = CellHorizontalAlignment.Center;
                this.fpSpread1.ActiveSheet.Cells[0, 0, columnHeaderRowLastIndex, columnCount - 1].BackColor = Color.LightGray;
                this.fpSpread1.ActiveSheet.SetOutlineBorder(new CellRange(0, 0, columnHeaderRowLastIndex + 1, columnCount), new ComplexBorder(thin, thin, thin, thin));
                this.fpSpread1.ActiveSheet.SetInsideBorder(new CellRange(0, 0, columnHeaderRowLastIndex + 1, columnCount), new ComplexBorder(none, none, thin, thin));
                this.fpSpread1.ActiveSheet.Rows[0, columnHeaderRowLastIndex].Font = new Font("Tahoma", 8.25F, FontStyle.Bold);
            }
        }
        void radMenuItemRowCount3_Click(object sender, EventArgs e)
        {
            if (columnHeaderRowLastIndex != 2)
            {
                this.columnHeaderRowLastIndex = 2;
                this.columnHeaderRowLabelIndex = 1;
                this.columnHeaderRowUnitIndex = 2;
                this.fpSpread1.ActiveSheet.RemoveRows(0, 1);
                this.fpSpread1.ActiveSheet.RowHeader.Rows[0].Label = "Preview";
                this.fpSpread1.ActiveSheet.RowHeader.Cells[0, 0].RowSpan = columnHeaderRowLastIndex + 1;
                var columnCount = this.fpSpread1.ActiveSheet.ColumnCount;
                var none = new ComplexBorderSide(Color.Gray, 0);
                var thin = new ComplexBorderSide(Color.Gray, 1); this.fpSpread1.ActiveSheet.Cells[0, 0, columnHeaderRowLastIndex, columnCount - 1].VerticalAlignment = CellVerticalAlignment.Center;
                this.fpSpread1.ActiveSheet.Cells[0, 0, columnHeaderRowLastIndex, columnCount - 1].HorizontalAlignment = CellHorizontalAlignment.Center;
                this.fpSpread1.ActiveSheet.Cells[0, 0, columnHeaderRowLastIndex, columnCount - 1].BackColor = Color.LightGray;
                this.fpSpread1.ActiveSheet.SetOutlineBorder(new CellRange(0, 0, columnHeaderRowLastIndex + 1, columnCount), new ComplexBorder(thin, thin, thin, thin));
                this.fpSpread1.ActiveSheet.SetInsideBorder(new CellRange(0, 0, columnHeaderRowLastIndex + 1, columnCount), new ComplexBorder(none, none, thin, thin));
                this.fpSpread1.ActiveSheet.Rows[0, columnHeaderRowLastIndex].Font = new Font("Tahoma", 8.25F, FontStyle.Bold);
            }
        }
        //PreviousButton
        private void radButtonPrevious_Click(object sender, EventArgs e)
        {
            CellRange cellRange;
            SheetView sheetView = this.fpSpread1.ActiveSheet;
            int iFrom = 0, iTo = 0;
            int i;

            try
            {
                cellRange = sheetView.GetSelection(0);
                //리스트에 아이템이 Attach를 제외하고 한개만 존재할때는 동작하지 않도록 함
                if (sheetView.ColumnCount < 2)
                    return;

                if (cellRange == null)
                    return;
                //선택된 아이템이 없는 경우 동작하지 않도록 함
                if (cellRange.ColumnCount == 0)
                    return;
                //선택된 아이템이 전체를 선택한 경우 동작하지 않도록 함
                if (cellRange.ColumnCount == sheetView.ColumnCount)
                    return;

                for (i = 0; i < cellRange.ColumnCount; i++)
                {
                    if (cellRange.Column - cellRange.ColumnCount < 0 || cellRange.Column == 0)
                        return;
                    iFrom = cellRange.Column + i;
                    iTo = iFrom - 1;
                    sheetView.MoveColumn(iFrom, iTo, true);
                }
                sheetView.ActiveColumnIndex = iTo;
                sheetView.AddSelection(cellRange.Row, iTo, cellRange.RowCount, cellRange.ColumnCount);
            }
            catch (Exception ex)
            {

            }
        }
        //NextButton
        private void radButtonNext_Click(object sender, EventArgs e)
        {
            CellRange cellRange;
            SheetView sheetView = this.fpSpread1.ActiveSheet;
            int iFrom = 0, iTo = 0;
            int i;

            try
            {
                cellRange = sheetView.GetSelection(0);
                //리스트에 아이템이 Attach를 제외하고 한개만 존재할때는 동작하지 않도록 함
                if (sheetView.ColumnCount < 2)
                    return;

                if (cellRange == null)
                    return;
                //선택된 아이템이 없는 경우 동작하지 않도록 함
                if (cellRange.ColumnCount == 0)
                    return;
                //선택된 아이템이 전체를 선택한 경우 동작하지 않도록 함
                if (cellRange.ColumnCount == sheetView.ColumnCount)
                    return;
                for (i = 0; i < cellRange.ColumnCount; i++)
                {
                    if (cellRange.Column + cellRange.ColumnCount == sheetView.ColumnCount)
                        return;
                    iFrom = cellRange.Column + cellRange.ColumnCount - 1 - i;
                    iTo = iFrom + 1;
                    sheetView.MoveColumn(iFrom, iTo, true);
                }
                sheetView.ActiveColumnIndex = iTo;
                sheetView.AddSelection(cellRange.Row, iTo, cellRange.RowCount, cellRange.ColumnCount);
            }
            catch (Exception ex)
            {

            }
        }
        public override void SaveData()
        {
            try
            {

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    this.MethodSaveColumnDynamic(this.CaseDynamic.CaseDynamicDefaultID, this.fpSpread1.ActiveSheetIndex);
                    this.MethodSaveColumnDynamicParent(this.CaseDynamic.CaseDynamicDefaultID, this.fpSpread1.ActiveSheetIndex);

                    foreach (var columnDynamicParent in this._columnDynamicParents)
                    {
                        if (this._columnDynamics[columnDynamicParent.Key].Where(w => w.DeleteFlag == false).Select(c => c.Label).Distinct().Count() != this._columnDynamics[columnDynamicParent.Key].Where(w => w.DeleteFlag == false).Count())
                        {
                            //중복 
                            return;
                        }

                        else if (this._columnDynamics[columnDynamicParent.Key].Where(w => w.DeleteFlag == false && string.IsNullOrWhiteSpace(w.Label)).Count() > 0)
                        {
                            //Key 누락.
                            return;
                        }
                    }

                    this.MethodSettingNewColumnDynamicID();


                    // 최종 DB 저장 귀찮아서 포문으로 함.
                    var sb = new StringBuilder();
                    foreach (var columnDynamicParent in this._columnDynamicParents)
                    {
                        if (columnDynamicParent.Value.ColumnDynamicParentDefaultID == 0)
                        {
                            if (columnDynamicParent.Value.ColumnHeaderMergeProperty != null)
                            {
                                sb.AppendFormat("Insert Into EDB_ColumnDynamicParentDefault (ListTypeID, casedynamicdefaultName,  ColumnHeaderMergeProperty) Values ({0}, '{1}', '{2}')"
                                   , this._entityEDBInformation.ListType.ListTypeID, columnDynamicParent.Value.CaseDynamicDefaultName, columnDynamicParent.Value.ColumnHeaderMergeProperty.Replace("'", "''"));
                            }
                        }
                        else
                        {
                            sb.AppendFormat("UPDATE EDB_ColumnDynamicParentDefault set ColumnHeaderMergeProperty = '{0}', modifiedDate = getdate() Where ColumnDynamicParentDefaultID = {1}", columnDynamicParent.Value.ColumnHeaderMergeProperty.Replace("'", "''"), columnDynamicParent.Value.ColumnDynamicParentDefaultID);
                        }
                        var columnDynamicNew = this._columnDynamics[columnDynamicParent.Key].Select(s => new EDB_ColumnDynamicDefault()
                        {
                            CaseDynamicDefaultName = s.CaseDynamicDefaultName,
                            CellType = s.CellType,
                            ColumnDynamicDefaultID = s.ColumnDynamicDefaultID,
                            ColumnIndex = s.ColumnIndex,
                            CommonUnitID = s.CommonUnitID,
                            DeleteFlag = s.DeleteFlag,
                            Label = s.Label,
                            LabelView = s.LabelView,
                            ListTypeID = s.ListTypeID,
                            Locked = s.Locked,
                            ModifiedDate = DateTime.Now,
                            Orders = s.Orders,
                            Width = s.Width,
                            BackColorArgb = s.BackColorArgb,
                            InstrumentProcessDataOwnershipID = s.InstrumentProcessDataOwnershipID,
                            OwnershipDisciplineID = s.OwnershipDisciplineID,
                            HorizontalAlignment = s.HorizontalAlignment,
                        }).ToList();
                        proxy.CreateColumnDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, columnDynamicNew, this._entityEDBInformation.ListType.ListTypeID, columnDynamicParent.Value.CaseDynamicDefaultName);
                    }
                    proxy.ExecuteNonQuery(EDCS.WF.BSL.Variable.SharedVariable.IsIE, sb);
                    this.MethodBindingRadDropDownListCaseDynamic();
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
        void fpSpread1_CellClick(object sender, CellClickEventArgs e)
        {
            if (e.Button.Equals(MouseButtons.Right))
            {
                if (!e.RowHeader && e.ColumnHeader && !e.ColumnFooter) // ColumnHeader Click
                {
                    var getSelections = fpSpread1.ActiveSheet.GetSelections();
                    bool isColumnSelected = default(bool);

                    var columnSelections = getSelections.Where(w => w.Row.Equals(-1) && w.RowCount.Equals(-1)).ToList();
                    if (columnSelections.Count > 0)
                    {
                        foreach (var columnSelection in columnSelections)
                        {
                            if (columnSelection.IntersectColumn(e.Column))
                            {
                                isColumnSelected = true;
                                break;
                            }
                        } // end foreach
                    }
                    if (!isColumnSelected) // Column 선택한 것들중에서 선택하지 않았다면
                    {
                        this.fpSpread1.ActiveSheet.ClearSelection();
                        this.fpSpread1.ActiveSheet.AddSelection(-1, e.Column, -1, 1);
                    }
                    this.radContextMenuColumn.Show(this.fpSpread1, e.X, e.Y);
                }
                else if (e.RowHeader && !e.ColumnHeader && !e.ColumnFooter) // RowHeader Click
                {
                    var getSelections = this.fpSpread1.ActiveSheet.GetSelections();
                    bool isRowSelected = default(bool);

                    var rowSelections = getSelections.Where(w => w.Column.Equals(-1) && w.ColumnCount.Equals(-1)).ToList();
                    if (rowSelections.Count > 0)
                    {
                        foreach (var rowSelection in rowSelections)
                        {
                            if (rowSelection.IntersectRow(e.Row))
                            {
                                isRowSelected = true;
                                break;
                            }
                        } // end foreach
                    }

                    if (!isRowSelected) // Row 선택한 것들중에서 선택하지 않았다면
                    {
                        this.fpSpread1.ActiveSheet.ClearSelection();
                        this.fpSpread1.ActiveSheet.AddSelection(e.Row, -1, 1, -1);
                    }
                    this.radContextMenuRowCount.Show(this.fpSpread1, e.X, e.Y); // Row ContextMenu 보여주기
                }
                else //cell
                {
                    bool isSelected = this.fpSpread1.ActiveSheet.IsSelected(e.Row, e.Column);
                    if (!isSelected) // Cell 선택한 것들중에서 선택하지 않았다면
                    {
                        this.fpSpread1.ActiveSheet.ClearSelection();
                        this.fpSpread1.ActiveSheet.AddSelection(e.Row, e.Column, 1, 1);
                    }
                    if (e.Row < this.columnHeaderRowLabelIndex)
                    {
                        this.radContextMenuMerge.Show(this.fpSpread1, e.X, e.Y);
                    }
                }
            }
        }
        void radMenuItemDeleteColumn_Click(object sender, EventArgs e)
        {
            var getSelections = fpSpread1.ActiveSheet.GetSelections();

            if (getSelections.Count() > 0)
            {
                foreach (var getSelection in getSelections.OrderByDescending(ob => ob.Column))
                {
                    if (getSelection.Row.Equals(-1) && getSelection.RowCount.Equals(-1)) // Column 선택
                    {
                        this.fpSpread1.ActiveSheet.RemoveColumns(getSelection.Column, getSelection.ColumnCount);
                    }
                } // end foreach
            }

        }
        void radMenuItemAddColumn_Click(object sender, EventArgs e)
        {
            var getSelections = fpSpread1.ActiveSheet.GetSelections();

            if (getSelections.Count() > 0)
            {
                foreach (var getSelection in getSelections.OrderByDescending(ob => ob.Column))
                {
                    if (getSelection.Row.Equals(-1) && getSelection.RowCount.Equals(-1)) // Column 선택
                    {
                        this.fpSpread1.ActiveSheet.AddColumns(getSelection.Column, getSelection.ColumnCount);
                    }
                } // end foreach
            }

            var columnCount = this.fpSpread1.ActiveSheet.ColumnCount;
            var none = new ComplexBorderSide(Color.Gray, 0);
            var thin = new ComplexBorderSide(Color.Gray, 1);
            this.fpSpread1.ActiveSheet.Cells[0, 0, columnHeaderRowLastIndex, columnCount - 1].VerticalAlignment = CellVerticalAlignment.Center;
            this.fpSpread1.ActiveSheet.Cells[0, 0, columnHeaderRowLastIndex, columnCount - 1].HorizontalAlignment = CellHorizontalAlignment.Center;
            this.fpSpread1.ActiveSheet.Cells[0, 0, columnHeaderRowLastIndex, columnCount - 1].BackColor = Color.LightGray;
            this.fpSpread1.ActiveSheet.SetOutlineBorder(new CellRange(0, 0, columnHeaderRowLastIndex + 1, columnCount), new ComplexBorder(thin, thin, thin, thin));
            this.fpSpread1.ActiveSheet.SetInsideBorder(new CellRange(0, 0, columnHeaderRowLastIndex + 1, columnCount), new ComplexBorder(none, none, thin, thin));
            this.fpSpread1.ActiveSheet.Rows[0, columnHeaderRowLastIndex].Font = new Font("Tahoma", 8.25F, FontStyle.Bold);
        }

        public void MethodBindingRadDropDownList()
        {

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                if (this.radDropDownListListType.SelectedItem == null)
                {
                    this.radDropDownListListType.ValueMember = "ListTypeID";
                    this.radDropDownListListType.DisplayMember = "ComplexNameView";
                    this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                    this.radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;

                    this.radDropDownListListType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListListType_SelectedIndexChanged);
                }

                this.MethodBindingRadDropDownListCaseDynamic();
            }
        }
        void radDropDownListListType_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                if (this.radDropDownListListType.SelectedItem != null)
                {
                    this._prevCaseDynamicID = 0;
                    this._entityEDBInformation.ListType = (EntityEDB_ListType)this.radDropDownListListType.SelectedItem.DataBoundItem;
                    this._commonColumnDynamics = proxy.ReadCommonColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, this._entityEDBInformation.ListType.ListTypeID);
                    this.MethodSettingDropDownListVisibleInstrument();
                    if (_entityEDBInformation.ListType.ListTypeID.Equals(ListTypeInstrumentProcessData) == false)
                    {
                        this.MethodBindingRadDropDownList();
                    }
                }
            }
        }

        void fpSpread1_Sheet1_CellChanged(object sender, SheetViewEventArgs e)
        {
            var sheetView = (SheetView)sender;

            int startRowIndex = e.Row;
            int rowCount = e.RowCount;
            int startColumnIndex = e.Column;
            int columnCount = e.ColumnCount;
            if (startRowIndex.Equals(-1))
            {
                startRowIndex = 0;
                rowCount = sheetView.RowCount;
            }
            if (startColumnIndex.Equals(-1))
            {
                startColumnIndex = 0;
                columnCount = sheetView.ColumnCount;
            }
            for (int i = startRowIndex; i < rowCount + startRowIndex; i++)
            {
                //if (sheetView.Rows[i].Tag == "CommonUnit")
                //{
                //    for (int j = startColumnIndex; j < columnCount + startColumnIndex; j++)
                //    {
                //        this.fpSpread1_Sheet1.CellChanged -= new SheetViewEventHandler(fpSpread1_Sheet1_CellChanged);
                //        sheetView.Cells[columnHeaderRowUnitIndex, j].Value = sheetView.Cells[i, j].Value;
                //        this.fpSpread1_Sheet1.CellChanged += new SheetViewEventHandler(fpSpread1_Sheet1_CellChanged);
                //    }
                //}
                //if (columnHeaderRowLabelIndex == i)
                //{
                //    for (int j = startColumnIndex; j < columnCount + startColumnIndex; j++)
                //    {
                //        this.fpSpread1_Sheet1.CellChanged -= new SheetViewEventHandler(fpSpread1_Sheet1_CellChanged);
                //        sheetView.Cells[sheetView.Rows["LabelView"].Index, j].Value = sheetView.Cells[columnHeaderRowLabelIndex, j].Value;
                //        this.fpSpread1_Sheet1.CellChanged += new SheetViewEventHandler(fpSpread1_Sheet1_CellChanged);
                //    }

                //}
                //else if (sheetView.Rows[i].Tag == "LabelView")
                //{
                //    for (int j = startColumnIndex; j < columnCount + startColumnIndex; j++)
                //    {
                //        this.fpSpread1_Sheet1.CellChanged -= new SheetViewEventHandler(fpSpread1_Sheet1_CellChanged);
                //        sheetView.Cells[columnHeaderRowLabelIndex, j].Value = sheetView.Cells[sheetView.Rows["LabelView"].Index, j].Value;
                //        this.fpSpread1_Sheet1.CellChanged += new SheetViewEventHandler(fpSpread1_Sheet1_CellChanged);
                //    }
                //}
            }
        }
        public void MethodBindingRadDropDownListCaseDynamic()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                this.fpSpread1.ActiveSheetChanged -= new EventHandler(fpSpread1_ActiveSheetChanged);
                this._prevCaseDynamicID = 0;
                this.CaseDynamic = null;
                this._columnDynamicParents = new Dictionary<int, EntityEDB_ColumnDynamicParentDefault>();
                this._originColumnDynamicParents = new Dictionary<int, EntityEDB_ColumnDynamicParentDefault>();

                this._columnDynamics = new Dictionary<int, List<EntityEDB_ColumnDynamicDefault>>();

                this._originColumnDynamics = new Dictionary<int, List<EntityEDB_ColumnDynamicDefault>>();

                this._tempColumnDynamicID = 0;

                int? basicTemplateID = null;
                int? specificTemplateID = null;

                if (_entityEDBInformation.ListType.ListTypeID.Equals(ListTypeInstrumentProcessData))
                {
                    if (radDropDownListBasicTemplate.SelectedIndex > -1)
                        basicTemplateID = (int)radDropDownListBasicTemplate.SelectedItem.Tag;
                    if (radDropDownListSpecificTemplate.SelectedIndex > -1)
                        specificTemplateID = (int)radDropDownListSpecificTemplate.SelectedItem.Tag;
                }

                var columnDynamicAllMax = proxy.ReadColumnDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE).Where(w => w.ListTypeID == this._entityEDBInformation.ListType.ListTypeID).ToList();
                if (columnDynamicAllMax.Count > 0) this._tempColumnDynamicID = columnDynamicAllMax.Max(m => m.ColumnDynamicDefaultID) + 1;

                this._lastTempColumnDynamicID = this._tempColumnDynamicID + 1;
                var caseDynamics = proxy.ReadCaseDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE).Where(w => w.VisibleFlag == true && w.ListTypeID == this._entityEDBInformation.ListType.ListTypeID
                    && (basicTemplateID == null ? true : w.InstrumentBasicTemplateID == basicTemplateID)
                    && (specificTemplateID == null ? true : w.InstrumentSpecificTemplateID == specificTemplateID)).ToList();

                this.fpSpread1.Sheets.Clear();
                var caseCount = caseDynamics.Count;

                var columnDynamicParentAll = proxy.ReadColumnDynamicParentDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.ListTypeID == this._entityEDBInformation.ListType.ListTypeID).ToList();
                var columnDynamicAll = proxy.ReadColumnDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE).Where(w => w.ListTypeID == this._entityEDBInformation.ListType.ListTypeID).ToList();

                for (int i = 0; i < caseCount; i++)
                {
                    var caseDynamic = caseDynamics[i];
                    this.fpSpread1.Sheets.Add(new SheetView());

                    this.fpSpread1.Sheets[i].SheetName = caseDynamic.NameView;
                    this.fpSpread1.Sheets[i].Tag = caseDynamic;

                    //default 는 전체 sheet 다 불러오자.
                    var columnDynamicParent = columnDynamicParentAll.SingleOrDefault(s => s.CaseDynamicDefaultName == caseDynamic.Name); ;
                    if (columnDynamicParent == null) columnDynamicParent = new EntityEDB_ColumnDynamicParentDefault();
                    columnDynamicParent.CaseDynamicDefaultName = caseDynamic.Name;

                    var columnDynamics = columnDynamicAll.Where(w => w.CaseDynamicDefaultName == caseDynamic.Name).ToList();

                    this._columnDynamicParents.Add(caseDynamic.CaseDynamicDefaultID, columnDynamicParent);
                    this._columnDynamics.Add(caseDynamic.CaseDynamicDefaultID, columnDynamics);

                    this._originColumnDynamicParents.Add(caseDynamic.CaseDynamicDefaultID, StaticMethod.GenericCopier<EntityEDB_ColumnDynamicParentDefault>.DeepCopy(columnDynamicParent));
                    this._originColumnDynamics.Add(caseDynamic.CaseDynamicDefaultID, StaticMethod.GenericCopier<List<EntityEDB_ColumnDynamicDefault>>.DeepCopy(columnDynamics));
                }

                this.fpSpread1.ActiveSheetChanged += new EventHandler(fpSpread1_ActiveSheetChanged);
                this.fpSpread1_ActiveSheetChanged(null, null);
            }
        }

        void fpSpread1_ActiveSheetChanged(object sender, EventArgs e)
        {
            if (this.CaseDynamic != null) this._prevCaseDynamicID = this.CaseDynamic.CaseDynamicDefaultID;
            this.CaseDynamic = (EntityEDB_CaseDynamicDefault)this.fpSpread1.ActiveSheet.Tag;
            this.MethodSettingRadGridView();
        }
        private int MethodGetSheetIndexByID(int p_caseDynamicID)
        {
            for (int i = 0; i < this.fpSpread1.Sheets.Count; i++)
            {
                var caseDynamic = this.fpSpread1.Sheets[i].Tag as EntityEDB_CaseDynamicDefault;
                if (caseDynamic.CaseDynamicDefaultID == p_caseDynamicID) return i;
            }
            return 0;
        }
        private void MethodSaveColumnDynamicParent(int p_caseDynamicID, int p_caseDynamicIndex)
        {
            // Merge
            var columnHeaderMergeProperties = new List<EntityColumnHeaderMergeProperty>();

            int rowCount = columnHeaderRowLastIndex + 1 - SharedVariable._constFixedColumnHeaderRowCount;
            int columnCount = this.fpSpread1.Sheets[p_caseDynamicIndex].ColumnCount;

            for (int i = 0; i < columnCount; i++)
            {
                for (int j = 0; j < rowCount; j++)
                {
                    var columnHeaderMergeProperty = new EntityColumnHeaderMergeProperty();

                    var cell = this.fpSpread1.Sheets[p_caseDynamicIndex].Cells.Get(j, i);
                    columnHeaderMergeProperty.RowIndex = j;
                    columnHeaderMergeProperty.ColumnIndex = i;
                    columnHeaderMergeProperty.RowSpan = cell.RowSpan;
                    columnHeaderMergeProperty.ColumnSpan = cell.ColumnSpan;
                    //columnHeaderMergeProperty.Visible = true;
                    columnHeaderMergeProperty.Text = cell.Text.IndexOf("##") >= 0 ? cell.Text.Substring(cell.Text.IndexOf("##") + 2) : cell.Text;

                    columnHeaderMergeProperties.Add(columnHeaderMergeProperty);
                } // end for
            } // end for

            this._columnDynamicParents[p_caseDynamicID].ColumnHeaderMergeProperty = XmlConverter.SerializeObject<List<
                                                            EntityColumnHeaderMergeProperty>>(columnHeaderMergeProperties);

            //// 병합 설정에서 Width를 수정하게 하므로, 병합 설정 이후, Width 저장
            //foreach (var columnDynamic in this._columnDynamics[p_caseDynamicID])
            //{
            //    if (columnDynamic.DeleteFlag.Equals(false))
            //    {
            //        columnDynamic.Width = this.fpSpread1.Sheets[p_caseDynamicIndex].Columns.Get(columnDynamic.ColumnIndex).Width;
            //        columnDynamic.LabelView = this.fpSpread1.Sheets[p_caseDynamicIndex].GetValue(columnHeaderRowLabelIndex, columnDynamic.ColumnIndex) == null ? "" : this.fpSpread1.Sheets[p_caseDynamicIndex].GetValue(columnHeaderRowLabelIndex, columnDynamic.ColumnIndex).ToString();
            //    }
            //} // end foreach
        }
        private void MethodSettingNewColumnDynamicID()
        {
            foreach (var columnDynamics in this._columnDynamics.Values)
            {
                foreach (var columnDynamic in columnDynamics)
                {
                    if (columnDynamic.ColumnDynamicDefaultID > this._tempColumnDynamicID)
                    {
                        columnDynamic.ColumnDynamicDefaultID = 0;
                    }
                }
            } // end foreach
        }

        private void MethodSaveColumnDynamic(int p_caseDynamicID, int p_caseDynamicIndex)
        {
            for (int i = 0; i < this._columnDynamics[p_caseDynamicID].Count; i++)
            {
                var columnDynamic = this._columnDynamics[p_caseDynamicID][i];
                if (columnDynamic.ColumnDynamicDefaultID > this._tempColumnDynamicID)
                {
                    this._columnDynamics[p_caseDynamicID].RemoveAt(i);
                    i--;
                }
                else
                    columnDynamic.DeleteFlag = true;
            } // end foreach
            var sheet = this.fpSpread1.Sheets[p_caseDynamicIndex];
            for (int columnIndex = 0; columnIndex < this.fpSpread1.Sheets[p_caseDynamicIndex].NonEmptyColumnCount; columnIndex++)
            {
                EntityEDB_ColumnDynamicDefault columnDynamic = null;
                var columnDynamicID = 0;
                if (sheet.Cells[sheet.Rows["ColumnDynamicID"].Index, columnIndex].Value != null)
                {
                    columnDynamicID = Convert.ToInt32(sheet.Cells[sheet.Rows["ColumnDynamicID"].Index, columnIndex].Value);
                }
                if (columnDynamicID == 0)
                {
                    columnDynamic = new EntityEDB_ColumnDynamicDefault();
                    columnDynamic.ColumnDynamicDefaultID = this._lastTempColumnDynamicID++;
                    columnDynamic.OwnershipDisciplineID = 18;

                    this._columnDynamics[p_caseDynamicID].Add(columnDynamic);
                }
                else
                {
                    columnDynamic = this._columnDynamics[p_caseDynamicID].SingleOrDefault(s => s.ColumnDynamicDefaultID == columnDynamicID);
                }
                columnDynamic.Orders = (columnIndex + 1) * 100;
                columnDynamic.ColumnIndex = columnIndex;
                columnDynamic.Label = sheet.Cells[sheet.Rows["Label"].Index, columnIndex].Value != null ? sheet.Cells[sheet.Rows["Label"].Index, columnIndex].Value.ToString() : "";
                columnDynamic.LabelView = sheet.Cells[sheet.Rows["LabelView"].Index, columnIndex].Value != null ? sheet.Cells[sheet.Rows["LabelView"].Index, columnIndex].Value.ToString() : "";
                columnDynamic.DeleteFlag = false;
                columnDynamic.ModifiedDate = DateTime.Now;
                columnDynamic.Width = sheet.Columns[columnIndex].Width;
                columnDynamic.ListTypeID = this._entityEDBInformation.ListType.ListTypeID;
                columnDynamic.CaseDynamicDefaultName = this._columnDynamicParents[p_caseDynamicID].CaseDynamicDefaultName;

                byte cellType = 0;
                if (sheet.Cells[sheet.Rows["CellType"].Index, columnIndex].Value != null && !string.IsNullOrWhiteSpace(sheet.Cells[sheet.Rows["CellType"].Index, columnIndex].Value.ToString()))
                {
                    cellType = Convert.ToByte(sheet.Cells[sheet.Rows["CellType"].Index, columnIndex].Value);

                    //var cellTypeString = sheet.Cells[sheet.Rows["CellType"].Index, columnIndex].Value.ToString();
                    //var enumCellType = this._enumCellTypes.SingleOrDefault(s => s.Text == cellTypeString);
                    //if (enumCellType != null) { cellType = enumCellType.Key; }
                }
                columnDynamic.CellType = cellType;

                var commonUnitID = 1;
                if (sheet.Cells[sheet.Rows["CommonUnit"].Index, columnIndex].Value != null && !string.IsNullOrWhiteSpace(sheet.Cells[sheet.Rows["CommonUnit"].Index, columnIndex].Value.ToString()))
                {
                    var CommonUnitString = sheet.Cells[sheet.Rows["CommonUnit"].Index, columnIndex].Value.ToString();
                    var commonUnit = this._commonUnits.SingleOrDefault(s => s.ComplexNameView == CommonUnitString);
                    if (commonUnit != null) { commonUnitID = commonUnit.CommonUnitID; }
                }
                columnDynamic.CommonUnitID = commonUnitID;

                bool locked = false;
                if (sheet.Cells[sheet.Rows["Lock"].Index, columnIndex].Value != null)
                {
                    locked = Convert.ToBoolean(sheet.Cells[sheet.Rows["Lock"].Index, columnIndex].Value);
                }
                columnDynamic.Locked = locked;

                byte horizontalAlignment = 0;
                if (sheet.Cells[sheet.Rows["HorizontalAlignment"].Index, columnIndex].Value != null)
                {
                    horizontalAlignment = Convert.ToByte(sheet.Cells[sheet.Rows["HorizontalAlignment"].Index, columnIndex].Value);
                }
                columnDynamic.HorizontalAlignment = horizontalAlignment == 0 ? null : new byte?(horizontalAlignment);

                //if (this._entityEDBInformation.ListType.ListTypeID == (int)EnumNameCheck.InstrumentList
                //    && sheet.Cells[sheet.Rows["BackColor"].Index, columnIndex].Value != null)
                //{
                //    Color backColor = (Color)sheet.Cells[sheet.Rows["BackColor"].Index, columnIndex].Value;
                //    columnDynamic.BackColorArgb = backColor.ToArgb();
                //}

                int? InstrumentProcessDataOwnershipID = null;
                if (this._entityEDBInformation.ListType.ListTypeID == (int)EnumNameCheck.InstrumentList
                    && sheet.Cells[sheet.Rows["InstrumentProcessDataOwnershipID"].Index, columnIndex].Value != null)
                {
                    var InstrumentProcessDataOwnershipString = sheet.Cells[sheet.Rows["InstrumentProcessDataOwnershipID"].Index, columnIndex].Value.ToString();
                    InstrumentProcessDataOwnershipID = _ownershipList.Where(w => w.Name == InstrumentProcessDataOwnershipString).First().InstrumentProcessDataOwnershipID;
                }
                columnDynamic.InstrumentProcessDataOwnershipID = InstrumentProcessDataOwnershipID;

                int OwnershipDisciplineID = 18;
                if (sheet.Cells[sheet.Rows["OwnershipDisciplineID"].Index, columnIndex].Value != null && !string.IsNullOrWhiteSpace(sheet.Cells[sheet.Rows["OwnershipDisciplineID"].Index, columnIndex].Value.ToString()))
                {
                    OwnershipDisciplineID = Convert.ToInt32(sheet.Cells[sheet.Rows["OwnershipDisciplineID"].Index, columnIndex].Value);
                }
                columnDynamic.OwnershipDisciplineID = OwnershipDisciplineID;
            }
        }
        //데이터 불러오기 , 이전값저장.
        private void MethodSettingRadGridView()
        {
            //sheet 가 변경되었을때 만 함. (projec,t listtype 이 변경되면초기화)
            if (this._prevCaseDynamicID != 0)
            {
                var prevCaseDynamicIndex = this.MethodGetSheetIndexByID(this._prevCaseDynamicID);
                this.MethodSaveColumnDynamicParent(this._prevCaseDynamicID, prevCaseDynamicIndex);
                this.MethodSaveColumnDynamic(this._prevCaseDynamicID, prevCaseDynamicIndex);
                //이전값 저장
            }
            //if (!this._columnDynamicParents.ContainsKey(this.CaseDynamic.CaseDynamicDefaultID))
            //{
            //    var columnDynamicParent = proxy.ReadColumnDynamicParentDefaults().;
            //    columnDynamicParent.ColumnDynamics = columnDynamicParent.ColumnDynamicsIQ.ToList();
            //    foreach (var columnDynamic in columnDynamicParent.ColumnDynamicsIQ)
            //    {
            //        columnDynamic.DisciplineColumnDynamics = columnDynamic.DisciplineColumnDynamicsIQ.ToList();
            //    }
            //    this._columnDynamicParents.Add(this.CaseDynamic.CaseDynamicDefaultID, columnDynamicParent);

            //    this._originColumnDynamicParents.Add(this.CaseDynamic.CaseDynamicDefaultID, new EntityEDB_ColumnDynamicParent()
            //    {
            //        ColumnDynamicParentID = this._columnDynamicParents[this.CaseDynamic.CaseDynamicDefaultID].ColumnDynamicParentID,
            //        CaseDynamicDefaultID = this._columnDynamicParents[this.CaseDynamic.CaseDynamicDefaultID].CaseDynamicDefaultID,
            //        VisibleFlag = this._columnDynamicParents[this.CaseDynamic.CaseDynamicDefaultID].VisibleFlag,
            //        DeleteFlag = this._columnDynamicParents[this.CaseDynamic.CaseDynamicDefaultID].DeleteFlag,
            //        ModifiedDate = this._columnDynamicParents[this.CaseDynamic.CaseDynamicDefaultID].ModifiedDate,
            //        ColumnHeaderMergeProperty = this._columnDynamicParents[this.CaseDynamic.CaseDynamicDefaultID].ColumnHeaderMergeProperty,
            //        ColumnDynamics = this._columnDynamicParents[this.CaseDynamic.CaseDynamicDefaultID].ColumnDynamics.Select(s => new EntityEDB_ColumnDynamic()
            //        {
            //            ColumnDynamicID = s.ColumnDynamicID,
            //            ColumnDynamicParentID = s.ColumnDynamicParentID,
            //            CommonUnitID = s.CommonUnitID,
            //            VisibleFlag = s.VisibleFlag,
            //            DeleteFlag = s.DeleteFlag,
            //            ModifiedDate = s.ModifiedDate,
            //            Orders = s.Orders,
            //            ColumnIndex = s.ColumnIndex,
            //            Label = s.Label,
            //            LabelView = s.LabelView,
            //            CellType = s.CellType,
            //            Locked = s.Locked,
            //            Width = s.Width,
            //            SpecSizeFlag = s.SpecSizeFlag,
            //        }).ToList(),
            //    });
            //}
            this.fpSpread1.ActiveSheet.CellChanged -= new SheetViewEventHandler(fpSpread1_Sheet1_CellChanged);
            this.MethodBindingRadGridView();
            this.fpSpread1.ActiveSheet.CellChanged += new SheetViewEventHandler(fpSpread1_Sheet1_CellChanged);
        }
        //실제 스프래드에 데이터 뿌리기.
        private void MethodBindingRadGridView()
        {
            fpSpread1.ActiveSheet.Columns.Clear();
            var columnCount = this._columnDynamics[this.CaseDynamic.CaseDynamicDefaultID].Where(w => w.DeleteFlag == false).Count();
            fpSpread1.ActiveSheet.ColumnCount = columnCount == 0 ? 10 : columnCount;

            this.MethodSettingColumnHeader();
            var rowCount = fpSpread1.ActiveSheet.RowCount;
            if (this._entityEDBInformation.ListType.ListTypeID == (int)EnumNameCheck.InstrumentList)
            {
                fpSpread1.ActiveSheet.Rows[rowCount - 9].Tag = "LabelView";
                fpSpread1.ActiveSheet.Rows[rowCount - 8].Tag = "CommonUnit";
                fpSpread1.ActiveSheet.Rows[rowCount - 7].Tag = "Label";
                fpSpread1.ActiveSheet.Rows[rowCount - 6].Tag = "CellType";
                fpSpread1.ActiveSheet.Rows[rowCount - 5].Tag = "Lock";
                fpSpread1.ActiveSheet.Rows[rowCount - 4].Tag = "InstrumentProcessDataOwnershipID";
                fpSpread1.ActiveSheet.Rows[rowCount - 3].Tag = "OwnershipDisciplineID";
                fpSpread1.ActiveSheet.Rows[rowCount - 2].Tag = "HorizontalAlignment";
                fpSpread1.ActiveSheet.Rows[rowCount - 1].Tag = "ColumnDynamicID";

                fpSpread1.ActiveSheet.RowHeader.Rows[0].Label = "Preview";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 9].Label = "HeaderName";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 8].Label = "UOM";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 7].Label = "Key";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 6].Label = "CellType";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 5].Label = "ReadOnly";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 4].Label = "Ownership";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 3].Label = "Discipline";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 2].Label = "Alignment";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 1].Label = "ColumnDynamicID";
            }
            else
            {
                fpSpread1.ActiveSheet.Rows[rowCount - 8].Tag = "LabelView";
                fpSpread1.ActiveSheet.Rows[rowCount - 7].Tag = "CommonUnit";
                fpSpread1.ActiveSheet.Rows[rowCount - 6].Tag = "Label";
                fpSpread1.ActiveSheet.Rows[rowCount - 5].Tag = "CellType";
                fpSpread1.ActiveSheet.Rows[rowCount - 4].Tag = "Lock";
                fpSpread1.ActiveSheet.Rows[rowCount - 3].Tag = "OwnershipDisciplineID";
                fpSpread1.ActiveSheet.Rows[rowCount - 2].Tag = "HorizontalAlignment";
                fpSpread1.ActiveSheet.Rows[rowCount - 1].Tag = "ColumnDynamicID";

                fpSpread1.ActiveSheet.RowHeader.Rows[0].Label = "Preview";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 8].Label = "HeaderName";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 7].Label = "UOM";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 6].Label = "Key";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 5].Label = "CellType";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 4].Label = "ReadOnly";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 3].Label = "Discipline";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 2].Label = "Alignment";
                fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 1].Label = "ColumnDynamicID";
            }

            fpSpread1.ActiveSheet.RowHeader.Rows[rowCount - 1].Visible = false;

            fpSpread1.ActiveSheet.RowHeader.Columns[0].Width = 70;

            this.MethodInitDropDownList();
            this.MethodSettingFpSpread();

            this.MethodDesignFpSpread();



        }
        //실제 데이터 넣는부분.
        private void MethodSettingFpSpread()
        {
            foreach (var columnDynamic in this._columnDynamics[this.CaseDynamic.CaseDynamicDefaultID].Where(w => w.DeleteFlag == false))
            {
                fpSpread1.ActiveSheet.Cells[fpSpread1.ActiveSheet.Rows["Label"].Index, columnDynamic.ColumnIndex].Value = columnDynamic.Label;
                fpSpread1.ActiveSheet.Cells[fpSpread1.ActiveSheet.Rows["LabelView"].Index, columnDynamic.ColumnIndex].Value = columnDynamic.LabelView;

                fpSpread1.ActiveSheet.Cells[fpSpread1.ActiveSheet.Rows["CellType"].Index, columnDynamic.ColumnIndex].Value = columnDynamic.CellType.ToString();// Enum.GetName(typeof(EnumCellType), columnDynamic.CellType);
                var commonUnit = this._commonUnits.SingleOrDefault(s => s.CommonUnitID == columnDynamic.CommonUnitID);
                fpSpread1.ActiveSheet.Cells[fpSpread1.ActiveSheet.Rows["CommonUnit"].Index, columnDynamic.ColumnIndex].Value = commonUnit == null ? "" : commonUnit.ComplexNameView;

                fpSpread1.ActiveSheet.Cells[fpSpread1.ActiveSheet.Rows["Lock"].Index, columnDynamic.ColumnIndex].Value = columnDynamic.Locked;
                fpSpread1.ActiveSheet.Cells[fpSpread1.ActiveSheet.Rows["HorizontalAlignment"].Index, columnDynamic.ColumnIndex].Value = columnDynamic.HorizontalAlignment;

                fpSpread1.ActiveSheet.Cells[fpSpread1.ActiveSheet.Rows["ColumnDynamicID"].Index, columnDynamic.ColumnIndex].Value = columnDynamic.ColumnDynamicDefaultID;

                //if (this._entityEDBInformation.ListType.ListTypeID == (int)EnumNameCheck.InstrumentList
                //    && columnDynamic.BackColorArgb != null)
                //{
                //    fpSpread1.ActiveSheet.Cells[fpSpread1.ActiveSheet.Rows["BackColor"].Index, columnDynamic.ColumnIndex].Value = Color.FromArgb(columnDynamic.BackColorArgb.Value);
                //}

                if (this._entityEDBInformation.ListType.ListTypeID == (int)EnumNameCheck.InstrumentList
                    && columnDynamic.InstrumentProcessDataOwnershipID != null)
                {
                    fpSpread1.ActiveSheet.Cells[fpSpread1.ActiveSheet.Rows["InstrumentProcessDataOwnershipID"].Index, columnDynamic.ColumnIndex].Value = this._ownershipList.Where(w => w.InstrumentProcessDataOwnershipID == columnDynamic.InstrumentProcessDataOwnershipID).First().Name;
                }

                fpSpread1.ActiveSheet.Cells[fpSpread1.ActiveSheet.Rows["OwnershipDisciplineID"].Index, columnDynamic.ColumnIndex].Value = columnDynamic.OwnershipDisciplineID.ToString();

            }
        }
        private void MethodSettingColumnHeader()
        {
            var textCellType = new TextCellType();
            textCellType.MaxLength = 99;
            textCellType.Multiline = true;

            var columnHeaderRowCount = SharedVariable._constFixedColumnHeaderRowCount + 1;

            List<EntityColumnHeaderMergeProperty> columnHeaderMergeProperties = null;
            if (!String.IsNullOrWhiteSpace(this._columnDynamicParents[this.CaseDynamic.CaseDynamicDefaultID].ColumnHeaderMergeProperty))
            {
                columnHeaderMergeProperties = XmlConverter.DeserializeObject<List<
                                       EntityColumnHeaderMergeProperty>>(this._columnDynamicParents[this.CaseDynamic.CaseDynamicDefaultID].ColumnHeaderMergeProperty);

                if (columnHeaderMergeProperties.Count > 0)
                {
                    columnHeaderRowCount = SharedVariable._constFixedColumnHeaderRowCount + columnHeaderMergeProperties.Max(m => m.RowIndex) + 1;
                    //this.radDropDownListRowCount.SelectedValue = columnHeaderMergeProperties.Max(m => m.RowIndex) + 1;
                }
            }
            var rowCount = columnHeaderRowCount + 6;
            if (this._entityEDBInformation.ListType.ListTypeID == (int)EnumNameCheck.InstrumentList)
                ++rowCount;
            var columnCount = fpSpread1.ActiveSheet.ColumnCount;
            fpSpread1.ActiveSheet.RowCount = rowCount;

            var newColumnDynamics = this._columnDynamics[this.CaseDynamic.CaseDynamicDefaultID].Where(w => w.DeleteFlag.Equals(false)).OrderBy(ob => ob.ColumnIndex).ToList(); // 컬럼 순서대로

            this.columnHeaderRowLastIndex = columnHeaderRowCount - 1;        // Merge Row
            this.columnHeaderRowLabelIndex = columnHeaderRowLastIndex - 1;   // Label Row
            this.columnHeaderRowUnitIndex = columnHeaderRowLastIndex;        // Unit  Row
            //new ComplexBorder(new ComplexBorderSide(){
            var none = new ComplexBorderSide(Color.Gray, 0);
            var thin = new ComplexBorderSide(Color.Gray, 1);
            this.fpSpread1.ActiveSheet.Cells[0, 0, columnHeaderRowCount - 1, columnCount - 1].VerticalAlignment = CellVerticalAlignment.Center;
            this.fpSpread1.ActiveSheet.Cells[0, 0, columnHeaderRowCount - 1, columnCount - 1].HorizontalAlignment = CellHorizontalAlignment.Center;
            this.fpSpread1.ActiveSheet.Cells[0, 0, columnHeaderRowCount - 1, columnCount - 1].BackColor = Color.LightGray;
            this.fpSpread1.ActiveSheet.SetOutlineBorder(new CellRange(0, 0, columnHeaderRowCount, columnCount), new ComplexBorder(thin, thin, thin, thin));
            this.fpSpread1.ActiveSheet.SetInsideBorder(new CellRange(0, 0, columnHeaderRowCount, columnCount), new ComplexBorder(none, none, thin, thin));
            this.fpSpread1.ActiveSheet.Rows[0, columnHeaderRowLastIndex].Font = new Font("Tahoma", 8.25F, FontStyle.Bold);
            this.fpSpread1.ActiveSheet.Rows[columnHeaderRowLastIndex + 1, rowCount - 1].Font = new Font("Tahoma", 8.25F);

            this.fpSpread1.ActiveSheet.RowHeader.Cells[0, 0].RowSpan = columnHeaderRowLastIndex + 1;

            for (int i = 0; i < newColumnDynamics.Count; i++)
            {
                int columnIndex = i;

                // ColumnHeaderFont
                this.fpSpread1.ActiveSheet.Columns[columnIndex].CellType = textCellType;

                //this.fpSpread1.Sheets[0].Columns[columnIndex].Font = new Font("Tahoma", 8.25F);
                //this.fpSpread1.Sheets[0].Columns[columnIndex].Tag = newColumnDynamics[i].VisibleFlag;
                // ColumnWidth
                if (newColumnDynamics[i].Width.HasValue)
                {
                    this.fpSpread1.ActiveSheet.SetColumnWidth(columnIndex, Convert.ToInt32(newColumnDynamics[i].Width.Value));
                }
                else
                {
                    this.fpSpread1.ActiveSheet.SetColumnWidth(columnIndex, 30);
                }
                for (int j = 0; j < columnHeaderRowCount; j++)
                {
                    int rowIndex = j;
                    this.fpSpread1.ActiveSheet.Cells.Get(rowIndex, columnIndex).Locked = false;

                    if (rowIndex.Equals(columnHeaderRowLabelIndex))     // Label
                    {
                        //머지창에서 컬럼 이름 변경가능
                        //this.fpSpread1.ActiveSheet.Cells.Get(rowIndex, columnIndex).Locked = true;
                        this.fpSpread1.ActiveSheet.SetValue(columnHeaderRowLabelIndex, columnIndex, newColumnDynamics[i].LabelView);
                    }
                    else if (rowIndex.Equals(columnHeaderRowUnitIndex)) // Unit
                    {
                        //this.fpSpread1.Sheets[0].Cells.Get(rowIndex, columnIndex).Locked = true;
                        //this.fpSpread1.Sheets[0].SetValue(columnHeaderRowUnitIndex, columnIndex, newColumnDynamics[i].CommonUnit.NameView);
                    }
                    else // Merge Label
                    {
                        // 등록된 XML이 있는지 체크.
                        if (!String.IsNullOrWhiteSpace(this._columnDynamicParents[this.CaseDynamic.CaseDynamicDefaultID].ColumnHeaderMergeProperty))
                        {
                            var columnHeaderMergeProperty = columnHeaderMergeProperties.Where(w => rowIndex.Equals(w.RowIndex) && columnIndex.Equals(w.ColumnIndex)).SingleOrDefault();

                            if (null != columnHeaderMergeProperty)
                            {
                                this.fpSpread1.ActiveSheet.Cells.Get(rowIndex, columnIndex).RowSpan = columnHeaderMergeProperty.RowSpan;
                                this.fpSpread1.ActiveSheet.Cells.Get(rowIndex, columnIndex).ColumnSpan = columnHeaderMergeProperty.ColumnSpan;
                                this.fpSpread1.ActiveSheet.SetValue(rowIndex, columnIndex, columnHeaderMergeProperty.Text);
                            }
                        }
                    }
                } // end for

                this.fpSpread1.ActiveSheet.SetMultipleRowHeights(0, this.fpSpread1.ActiveSheet.RowCount, 30);

            }
        }
        //초기 드랍다운리스트 . celltype 설정.
        private void MethodInitDropDownList()
        {
            var comboBoxCellType = new ComboBoxCellType();
            ListBox listBox = new ListBox();
            #region Label
            var commonColumnDynamicName = this._commonColumnDynamics.Select(s => s.Name).ToArray(); ;
            comboBoxCellType = new ComboBoxCellType();
            listBox = new ListBox();
            listBox.Items.AddRange(commonColumnDynamicName);
            listBox.Font = new Font("Tahoma", 7F);
            listBox.BorderStyle = BorderStyle.Fixed3D;
            comboBoxCellType.ListWidth = 150;
            comboBoxCellType.ListControl = listBox;
            comboBoxCellType.Editable = true;
            comboBoxCellType.MaxDrop = 15;
            comboBoxCellType.AutoCompleteMode = AutoCompleteMode.Append;
            comboBoxCellType.DropDownWhenStartEditing = false;
            comboBoxCellType.DropDownOptions = DropDownOptions.Button;
            this.fpSpread1.ActiveSheet.Rows["Label"].CellType = comboBoxCellType;
            #endregion
            #region LabelView
            var commonColumnDynamicNameView = this._commonColumnDynamics.Select(s => s.NameView).ToArray(); ;
            comboBoxCellType = new ComboBoxCellType();
            listBox = new ListBox();
            listBox.Items.AddRange(commonColumnDynamicNameView);
            listBox.Font = new Font("Tahoma", 8F);
            listBox.BorderStyle = BorderStyle.Fixed3D;
            comboBoxCellType.ListWidth = 150;
            comboBoxCellType.ListControl = listBox;
            comboBoxCellType.Editable = true;
            comboBoxCellType.MaxDrop = 15;
            comboBoxCellType.AutoCompleteMode = AutoCompleteMode.Append;
            comboBoxCellType.DropDownWhenStartEditing = false;
            comboBoxCellType.DropDownOptions = DropDownOptions.Button;
            this.fpSpread1.ActiveSheet.Rows["LabelView"].CellType = comboBoxCellType;
            #endregion
            #region Type
            var cellTypes = this._enumCellTypes.Select(s => s.Text).ToArray();
            comboBoxCellType = new ComboBoxCellType();
            comboBoxCellType.Items = this._enumCellTypes.Select(s => s.Text).ToArray();
            comboBoxCellType.ItemData = this._enumCellTypes.Select(s => s.Key.ToString()).ToArray();
            comboBoxCellType.EditorValue = EditorValue.ItemData;
            //comboBoxCellType.ListControl = listBox;
            comboBoxCellType.ListWidth = 130;
            comboBoxCellType.Editable = false;
            comboBoxCellType.MaxDrop = 15;
            comboBoxCellType.AutoCompleteMode = AutoCompleteMode.Append;
            comboBoxCellType.DropDownWhenStartEditing = false;
            comboBoxCellType.DropDownOptions = DropDownOptions.Button;
            this.fpSpread1.ActiveSheet.Rows["CellType"].CellType = comboBoxCellType;
            #endregion
            #region CommonUnit
            comboBoxCellType = new ComboBoxCellType();
            listBox = new ListBox();
            listBox.Items.AddRange(this._commonUnits.OrderBy(ob=>ob.CommonUnitID).Select(s => s.ComplexNameView).ToArray());
            listBox.BorderStyle = BorderStyle.Fixed3D;
            comboBoxCellType.ListControl = listBox;
            comboBoxCellType.ListWidth = 200;
            comboBoxCellType.Editable = false;
            comboBoxCellType.MaxDrop = 15;
            comboBoxCellType.AutoCompleteMode = AutoCompleteMode.Append;
            comboBoxCellType.DropDownWhenStartEditing = false;
            comboBoxCellType.DropDownOptions = DropDownOptions.Button;
            this.fpSpread1.ActiveSheet.Rows["CommonUnit"].CellType = comboBoxCellType;
            #endregion
            #region Lock
            var checkBox = new CheckBoxCellType();
            this.fpSpread1.ActiveSheet.Rows["Lock"].CellType = checkBox;
            #endregion
            #region ColorPicker
            //if (this._entityEDBInformation.ListType.ListTypeID == (int)EnumNameCheck.InstrumentList)
            //{
            //    var colorPicker = new ColorPickerCellType();
            //    this.fpSpread1.ActiveSheet.Rows["BackColor"].CellType = colorPicker;
            //}
            #endregion
            #region Ownership
            if (this._entityEDBInformation.ListType.ListTypeID == (int)EnumNameCheck.InstrumentList)
            {
                comboBoxCellType = new ComboBoxCellType();
                listBox = new ListBox();
                foreach (var ownership in _ownershipList)
                {
                    listBox.Items.Add(ownership.Name);
                }
                listBox.BorderStyle = BorderStyle.Fixed3D;
                comboBoxCellType.ListControl = listBox;
                comboBoxCellType.ListWidth = 130;
                comboBoxCellType.Editable = false;
                comboBoxCellType.MaxDrop = 15;
                comboBoxCellType.AutoCompleteMode = AutoCompleteMode.Append;
                comboBoxCellType.DropDownWhenStartEditing = false;
                comboBoxCellType.DropDownOptions = DropDownOptions.Button;
                this.fpSpread1.ActiveSheet.Rows["InstrumentProcessDataOwnershipID"].CellType = comboBoxCellType;
            }
            #endregion
            #region Discipline
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                comboBoxCellType = new ComboBoxCellType();
                listBox = new ListBox();
                var _disciplineUse = new System.Collections.Generic.List<EntityEDB_Discipline>();
                var _disciplineAll = proxy.ReadDisciplines(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                EDCS.WF.BSL.Method.SharedMethod.MethodSettingDisciplineUse(ref _disciplineUse, this._entityEDBInformation.ListType.ListTypeID, _disciplineAll, null);
                // Add Other
                _disciplineUse.Add(_disciplineAll.Where(w => w.DisciplineID.Equals(18)).First());
                listBox.BorderStyle = BorderStyle.Fixed3D;
                comboBoxCellType.Items = _disciplineUse.Select(s => s.NameView).ToArray();
                comboBoxCellType.ItemData = _disciplineUse.Select(s => s.DisciplineID.ToString()).ToArray();
                comboBoxCellType.EditorValue = EditorValue.ItemData;
                //comboBoxCellType.ListControl = listBox;
                comboBoxCellType.ListWidth = 130;
                comboBoxCellType.Editable = false;
                comboBoxCellType.MaxDrop = 15;
                comboBoxCellType.AutoCompleteMode = AutoCompleteMode.Append;
                comboBoxCellType.DropDownWhenStartEditing = false;
                comboBoxCellType.DropDownOptions = DropDownOptions.Button;
                this.fpSpread1.ActiveSheet.Rows["OwnershipDisciplineID"].CellType = comboBoxCellType;
            }
            #endregion
            #region HorizontalAlignment
            comboBoxCellType = new ComboBoxCellType();
            listBox = new ListBox();
            var cellHorizontalAlignmentListText = Enum.GetNames(typeof(FarPoint.Win.Spread.CellHorizontalAlignment)).ToList();
            var cellHorizontalAlignmentListValue = ((FarPoint.Win.Spread.CellHorizontalAlignment[])Enum.GetValues(typeof(FarPoint.Win.Spread.CellHorizontalAlignment))).Select(s => ((int)s).ToString()).ToArray();
            cellHorizontalAlignmentListText[0] = "";

            // Add Other
            listBox.BorderStyle = BorderStyle.Fixed3D;
            comboBoxCellType.Items = cellHorizontalAlignmentListText.ToArray();
            comboBoxCellType.ItemData = cellHorizontalAlignmentListValue;
            comboBoxCellType.EditorValue = EditorValue.ItemData;
            comboBoxCellType.ListWidth = 130;
            comboBoxCellType.Editable = false;
            comboBoxCellType.MaxDrop = 15;
            comboBoxCellType.AutoCompleteMode = AutoCompleteMode.Append;
            comboBoxCellType.DropDownWhenStartEditing = false;
            comboBoxCellType.DropDownOptions = DropDownOptions.Button;
            this.fpSpread1.ActiveSheet.Rows["HorizontalAlignment"].CellType = comboBoxCellType;
            #endregion
        }

        private void MethodDesignForm()
        {


            this.radLabel2.Font = new Font("Tahoma", 9F, FontStyle.Bold);

            this.radLabel2.ForeColor = Color.DarkSlateGray;
            // radGridView1

            this.Size = new Size(1000, 800);
            this.Text = "BQ Mapping";




        }
        private void MethodDesignFpSpread()
        {
            //this.radGridView1.AllowRowReorder = true;
            InputMap focusedMap = fpSpread1.GetInputMap(InputMapMode.WhenFocused);
            focusedMap.Put(new Keystroke(Keys.Delete, Keys.None), SpreadActions.ClipboardCutValues);
            focusedMap.Put(new Keystroke(Keys.V, Keys.Control), SpreadActions.ClipboardPasteValues);
            focusedMap.Put(new Keystroke(Keys.C, Keys.Control), SpreadActions.ClipboardCopyValues);
            focusedMap.Put(new Keystroke(Keys.X, Keys.Control), SpreadActions.ClipboardCutValues);

            focusedMap.Put(new Keystroke(Keys.Left, Keys.None), SpreadActions.MoveToPreviousColumn);
            focusedMap.Put(new Keystroke(Keys.Right, Keys.None), SpreadActions.MoveToNextColumn);
            focusedMap.Put(new Keystroke(Keys.Up, Keys.None), SpreadActions.MoveToPreviousRow);
            focusedMap.Put(new Keystroke(Keys.Down, Keys.None), SpreadActions.MoveToNextRow);


            focusedMap.Put(new Keystroke(Keys.Enter, Keys.None), SpreadActions.MoveToNextRow);
            focusedMap.Put(new Keystroke(Keys.Tab, Keys.None), SpreadActions.MoveToNextColumn);
            focusedMap.Put(new Keystroke(Keys.F2, Keys.None), SpreadActions.DeleteShape);
            focusedMap.Put(new Keystroke(Keys.Escape, Keys.None), SpreadActions.CancelEditing);

            this.fpSpread1.ClipboardOptions = ClipboardOptions.NoHeaders;

            this.fpSpread1.AllowCellOverflow = false;
            this.fpSpread1.AllowEditOverflow = false;

            //this.fpSpread1.ImeMode = System.Windows.Forms.ImeMode.On;
            this.fpSpread1.AllowColumnMove = true;
            this.fpSpread1.AllowColumnMoveMultiple = true;
            this.fpSpread1.AllowRowMove = false;
            this.fpSpread1.ButtonDrawMode = ButtonDrawModes.CurrentCell;

            this.fpSpread1.ScrollBarTrackPolicy = ScrollBarTrackPolicy.Both;
            this.fpSpread1.TabStripPolicy = TabStripPolicy.Always;

            this.fpSpread1.TabStripInsertTab = false; // TabStrip 추가하지 못하도록 설정

        }

        private void MethodSettingDropDownListVisibleInstrument()
        {
            if (_entityEDBInformation.ListType.ListTypeID.Equals(ListTypeInstrumentProcessData))
            {
                radLabelProjectType.Visible = true;
                radDropDownListProjectType.Visible = true;

                radLabelBasicTemplate.Visible = true;
                radDropDownListBasicTemplate.Visible = true;

                radLabelSpecificTemplate.Visible = true;
                radDropDownListSpecificTemplate.Visible = true;

                radDropDownListProjectType.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListProjectType_SelectedIndexChanged);
                radDropDownListProjectType.Items.Clear();

                RadListDataItem item = new RadListDataItem();
                item.Tag = (int)EnumProjectType2.Chemical_Plant;
                item.Text = Enum.GetName(typeof(EnumProjectType2), EnumProjectType2.Chemical_Plant);
                radDropDownListProjectType.Items.Add(item);

                item = new RadListDataItem();
                item.Tag = (int)EnumProjectType2.Power_Plant;
                item.Text = Enum.GetName(typeof(EnumProjectType2), EnumProjectType2.Power_Plant);
                radDropDownListProjectType.Items.Add(item);

                radDropDownListProjectType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListProjectType_SelectedIndexChanged);
                radDropDownListProjectType.SelectedIndex = 0;
            }
            else
            {
                radLabelProjectType.Visible = false;
                radDropDownListProjectType.Visible = false;

                radLabelBasicTemplate.Visible = false;
                radDropDownListBasicTemplate.Visible = false;

                radLabelSpecificTemplate.Visible = false;
                radDropDownListSpecificTemplate.Visible = false;
            }
        }

        private void radDropDownListProjectType_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            try
            {

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    var InstrumentBasicTemplates = proxy.ReadInstrumentBasicTemplate(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                    radDropDownListBasicTemplate.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListBasicTemplate_SelectedIndexChanged);
                    radDropDownListBasicTemplate.Items.Clear();
                    foreach (var InstrumentBasicTemplate in InstrumentBasicTemplates)
                    {
                        RadListDataItem item = new RadListDataItem();
                        item.Tag = InstrumentBasicTemplate.InstrumentBasicTemplateID;
                        item.Text = InstrumentBasicTemplate.NameView;
                        radDropDownListBasicTemplate.Items.Add(item);
                    }
                    radDropDownListBasicTemplate.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListBasicTemplate_SelectedIndexChanged);
                    radDropDownListBasicTemplate.SelectedIndex = 0;
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

        private void radDropDownListBasicTemplate_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    if (_entityEDBInformation.ListType.ListTypeID.Equals(ListTypeInstrumentProcessData))
                    {
                        if (radDropDownListBasicTemplate.SelectedIndex > -1)
                        {
                            int basicTemplateID = (int)radDropDownListBasicTemplate.SelectedItem.Tag;
                            int projectType = 1;
                            if (this.radDropDownListProjectType.SelectedItem != null)
                                projectType = (int)this.radDropDownListProjectType.SelectedItem.Tag;

                            var InstrumentSpecificTemplates = proxy.ReadInstrumentSpecificTemplate(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, basicTemplateID, projectType);
                            radDropDownListSpecificTemplate.Items.Clear();
                            foreach (var InstrumentSpecificTemplate in InstrumentSpecificTemplates)
                            {
                                RadListDataItem item = new RadListDataItem();
                                item.Tag = InstrumentSpecificTemplate.InstrumentSpecificTemplateID;
                                item.Text = InstrumentSpecificTemplate.NameView;
                                radDropDownListSpecificTemplate.Items.Add(item);
                            }
                            radDropDownListSpecificTemplate.SelectedIndex = 0;
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

        private void radDropDownListSpecificTemplate_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            if (radDropDownListSpecificTemplate.Items.Count > 0)
                MethodBindingRadDropDownList();
        }
    }
}

