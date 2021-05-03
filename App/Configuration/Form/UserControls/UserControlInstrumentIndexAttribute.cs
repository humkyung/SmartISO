using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.CellType;
using FarPoint.Win.Spread.Model;

namespace Configuration
{
    public partial class UserControlInstrumentIndexAttribute : UserControlBase
    {
        private enum LTColumn
        {
            InstrumentIndexAttributeID, Label, LabelView, RelationTable,
            RelationColumn
                , TargetTable, TargetKeyColumn, SourceTable, SourceKeyColumn, DBUpdate, Import, ReadOnly, V2007, V2009, V2013, V2016, Remark
        };
        private string[] mColumnName = new string[] { " ", "Label", "LabelView", "RelationTable", "RelationColumn"
            , "TargetTable", "TargetKeyColumn", "SourceTable", "SourceKeyColumn", "DBUpdate", "Import", "ReadOnly", "V2007", "V2009", "V2013", "V2016", "Remark" };
        private int[] marColumnWidths = new int[] { 0, 150, 150, 150, 150, 150, 150, 150, 150, 90, 80, 90, 80, 80, 80, 80, 80 };

        public UserControlInstrumentIndexAttribute()
        {
            InitializeComponent();
        }

        public override void LoadData()
        {
            InitSetting();
        }

        private void InitSetting()
        {
            InitSpread();
            setSpread();

            fpSpreadData.ActiveSheet.FrozenColumnCount = 3;
            LineBorder borderColumn = new LineBorder(Color.Black, 1, false, false, true, false);

            for (int nRow = 0; nRow < fpSpreadData.ActiveSheet.RowCount; nRow++)
            {
                fpSpreadData.ActiveSheet.Cells[nRow, fpSpreadData.ActiveSheet.FrozenColumnCount - 1].Border = borderColumn;
            }
        }

        private void InitSpread()
        {
            SpreadUtil.InitSpread(fpSpreadData, marColumnWidths, mColumnName, null, null, null, null, null, null, Color.LightGray, true, true);

            for (int columnIndex = 0; columnIndex < fpSpreadData.ActiveSheet.Columns.Count; columnIndex++)
            {
                if (columnIndex == (int)LTColumn.DBUpdate
                    || columnIndex == (int)LTColumn.Import
                    || columnIndex == (int)LTColumn.ReadOnly
                    || columnIndex == (int)LTColumn.V2007
                    || columnIndex == (int)LTColumn.V2009
                    || columnIndex == (int)LTColumn.V2013
                    || columnIndex == (int)LTColumn.V2016
                    )
                {
                    CheckBoxCellType ctCheckBox = new CheckBoxCellType();
                    fpSpreadData.ActiveSheet.Columns[columnIndex].CellType = ctCheckBox;
                    fpSpreadData.ActiveSheet.Columns[columnIndex].HorizontalAlignment = CellHorizontalAlignment.Center;
                }
            }
        }

        private void setSpread()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var InstrumentIndexAttributes = proxy.ReadInstrumentIndexAttributes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                fpSpreadData.ActiveSheet.Rows.Count = InstrumentIndexAttributes.Count;

                for (int rowIndex = 0; rowIndex < InstrumentIndexAttributes.Count; rowIndex++)
                {
                    var instrumentIndexAttribute = InstrumentIndexAttributes[rowIndex];
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.InstrumentIndexAttributeID].Value = instrumentIndexAttribute.InstrumentIndexAttributeID;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.Label].Value = instrumentIndexAttribute.Label;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.LabelView].Value = instrumentIndexAttribute.LabelView;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.RelationTable].Value = instrumentIndexAttribute.RelationTable;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.RelationColumn].Value = instrumentIndexAttribute.RelationColumn;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.TargetTable].Value = instrumentIndexAttribute.TargetTable;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.TargetKeyColumn].Value = instrumentIndexAttribute.TargetKeyColumn;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.SourceTable].Value = instrumentIndexAttribute.SourceTable;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.SourceKeyColumn].Value = instrumentIndexAttribute.SourceKeyColumn;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.DBUpdate].Value = instrumentIndexAttribute.DBUpdate;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.Import].Value = instrumentIndexAttribute.Import;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.ReadOnly].Value = instrumentIndexAttribute.ReadOnly;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.V2007].Value = instrumentIndexAttribute.V2007;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.V2009].Value = instrumentIndexAttribute.V2009;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.V2013].Value = instrumentIndexAttribute.V2013;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.V2016].Value = instrumentIndexAttribute.V2016;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.Remark].Value = instrumentIndexAttribute.Remark;
                }
            }
        }

        private void radButtonCreate_Click(object sender, EventArgs e)
        {
            int iRow = fpSpreadData.ActiveSheet.Rows.Count++;
        }

        private void radButtonDelete_Click(object sender, EventArgs e)
        {
            fpSpreadData.ActiveSheet.Rows.Remove(fpSpreadData.ActiveSheet.ActiveRowIndex, 1);
        }

        public override void SaveData()
        {
            string sMessage = string.Empty;

            if (CheckSaveAttribute())
            {
                SaveAttribute(ref sMessage);

                if (sMessage.Length > 0)
                    MessageBox.Show(sMessage.ToString());
                else
                    MessageBox.Show(this, "Saved successfully!", this.Text);
            }
        }

        public bool CheckSaveAttribute()
        {
            bool bResult = true;

            List<string> lstLabel = new List<string>();

            Dictionary<int, string> dicoverlap_Item = new Dictionary<int, string>();

            for (int i = 0; i < fpSpreadData.ActiveSheet.RowCount; i++)
            {
                lstLabel.Add(fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.Label].Text);
            }

            int iRow = 0;
            foreach (string sItem in lstLabel.Distinct().ToArray())
            {
                if (!string.IsNullOrEmpty(sItem) && lstLabel.Count(n => n == sItem) > 1)
                {
                    if (!dicoverlap_Item.ContainsKey(iRow))
                        dicoverlap_Item.Add(iRow, string.Format("Label : {0}", sItem));
                }

                iRow++;
            }

            if (dicoverlap_Item.Count > 0)
            {
                bResult = false;

                StringBuilder sbMessage = new StringBuilder();
                sbMessage.Append("Duplicate Items");
                foreach (KeyValuePair<int, string> kv in dicoverlap_Item)
                {
                    sbMessage.Append(string.Format("Item : {0}", kv.Value));
                }

                MessageBox.Show(this, sbMessage.ToString(), this.Text);
            }

            return bResult;
        }

        public void SaveAttribute(ref string sMessage)
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                List<EDB_InstrumentIndexAttribute> InstrumentIndexAttributes = new List<EDB_InstrumentIndexAttribute>();

                for (int rowIndex = 0; rowIndex < fpSpreadData.ActiveSheet.Rows.Count; rowIndex++)
                {
                    var instrumentIndexAttribute = new EDB_InstrumentIndexAttribute();
                    instrumentIndexAttribute.DeleteFlag = false;
                    instrumentIndexAttribute.ModifiedDate = DateTime.Now;
                    instrumentIndexAttribute.Orders = (rowIndex + 1) * 100;
                    instrumentIndexAttribute.InstrumentIndexAttributeID = 0;
                    instrumentIndexAttribute.Label = fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.Label].Text;
                    instrumentIndexAttribute.LabelView = fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.LabelView].Text;
                    instrumentIndexAttribute.RelationTable = fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.RelationTable].Text;
                    instrumentIndexAttribute.RelationColumn = fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.RelationColumn].Text;
                    instrumentIndexAttribute.TargetTable = fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.TargetTable].Text;
                    instrumentIndexAttribute.TargetKeyColumn = fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.TargetKeyColumn].Text;
                    instrumentIndexAttribute.SourceTable = fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.SourceTable].Text;
                    instrumentIndexAttribute.SourceKeyColumn = fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.SourceKeyColumn].Text;
                    instrumentIndexAttribute.DBUpdate = (bool)fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.DBUpdate].Value;
                    instrumentIndexAttribute.Import = (bool)fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.Import].Value;
                    instrumentIndexAttribute.ReadOnly = (bool)fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.ReadOnly].Value;
                    instrumentIndexAttribute.V2007 = (bool)fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.V2007].Value;
                    instrumentIndexAttribute.V2009 = (bool)fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.V2009].Value;
                    instrumentIndexAttribute.V2013 = (bool)fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.V2013].Value;
                    instrumentIndexAttribute.V2016 = (bool)fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.V2016].Value;
                    instrumentIndexAttribute.Remark = fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.Remark].Text;
                    InstrumentIndexAttributes.Add(instrumentIndexAttribute);
                }

                proxy.CreateInstrumentIndexAttribute(EDCS.WF.BSL.Variable.SharedVariable.IsIE, InstrumentIndexAttributes);
            }
        }

        private void radButtonUp_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadData.ActiveSheet;

            int iFrom = 0, iTo = 0;
            int i;

            try
            {
                cr = sheetView.GetSelection(0);
                //리스트에 아이템이 Attach를 제외하고 한개만 존재할때는 동작하지 않도록 함
                if (sheetView.RowCount < 2)
                    return;
                if (cr == null)
                    return;
                //선택된 아이템이 없는 경우 동작하지 않도록 함
                if (cr.RowCount == 0)
                    return;
                //선택된 아이템이 전체를 선택한 경우 동작하지 않도록 함
                if (cr.RowCount == sheetView.RowCount)
                    return;

                for (i = 0; i < cr.RowCount; i++)
                {
                    if ((cr.Row + i) == 0)
                        return;
                    iFrom = cr.Row + i;
                    iTo = iFrom - 1;
                    sheetView.MoveRow(iFrom, iTo, true);
                }
                sheetView.ActiveRowIndex = cr.Row - 1;
                sheetView.AddSelection(cr.Row - 1, cr.Column, cr.RowCount, cr.ColumnCount);

            }
            catch (Exception ex)
            {

            }
        }

        private void radButtonDown_Click(object sender, EventArgs e)
        {
            CellRange cr;
            SheetView sheetView = fpSpreadData.ActiveSheet;
            int iFrom = 0, iTo = 0;
            int i;

            try
            {
                cr = sheetView.GetSelection(0);
                //리스트에 아이템이 Attach를 제외하고 한개만 존재할때는 동작하지 않도록 함
                if (sheetView.RowCount < 2)
                    return;

                if (cr == null)
                    return;
                //선택된 아이템이 없는 경우 동작하지 않도록 함
                if (cr.RowCount == 0)
                    return;
                //선택된 아이템이 전체를 선택한 경우 동작하지 않도록 함
                if (cr.RowCount == sheetView.RowCount)
                    return;

                for (i = 0; i < cr.RowCount; i++)
                {
                    if (cr.Row + cr.RowCount == sheetView.RowCount)
                        return;
                    iFrom = cr.Row + cr.RowCount - 1 - i;
                    iTo = iFrom + 1;
                    sheetView.MoveRow(iFrom, iTo, true);
                }
                sheetView.ActiveRowIndex = iTo;
                sheetView.AddSelection(iTo, cr.Column, cr.RowCount, cr.ColumnCount);

            }
            catch (Exception ex)
            {

            }
        }
    }
}
