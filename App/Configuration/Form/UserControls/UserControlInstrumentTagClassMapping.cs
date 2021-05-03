using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;
using FarPoint.Win.Spread.Model;

namespace Configuration
{
    public partial class UserControlInstrumentTagClassMapping : UserControlBase
    {
        private enum LTColumn { TagCategoryID, TagCategoryName, TagConvTypeID, TagConvTypeName };
        private string[] mColumnName = new string[] { "Tag Class ID", "Tag Class Name", "Tag Convention Type ID", "Tag Convention Type Name" };
        private int[] marColumnWidths = new int[] { 200, 250, 200, 250 };

        public UserControlInstrumentTagClassMapping()
        {
            InitializeComponent();
        }

        public override void LoadData()
        {
            try
            {
                InitSetting();
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

        private void InitSetting()
        {
            InitSpread();
            setSpread();
        }

        private void InitSpread()
        {
            SpreadUtil.InitSpread(fpSpreadData, marColumnWidths, mColumnName, null, null, null, null, null, null, Color.LightGray, true, true);
        }

        private void setSpread()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var InstrumentTagClassMappings = proxy.ReadInstrumentTagClassMappings(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);

                fpSpreadData.ActiveSheet.Rows.Count = InstrumentTagClassMappings.Count;

                for (int rowIndex = 0; rowIndex < InstrumentTagClassMappings.Count; rowIndex++)
                {
                    var instrumentTagClassMapping = InstrumentTagClassMappings[rowIndex];
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.TagCategoryID].Value = instrumentTagClassMapping.TagCategoryID;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.TagCategoryName].Value = instrumentTagClassMapping.TagCategoryName;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.TagConvTypeID].Value = instrumentTagClassMapping.TagConvTypeID;
                    fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.TagConvTypeName].Value = instrumentTagClassMapping.TagConvTypeName;
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
                lstLabel.Add(fpSpreadData.ActiveSheet.Cells[i, (int)LTColumn.TagCategoryID].Text);
            }

            int iRow = 0;
            foreach (string sItem in lstLabel.Distinct().ToArray())
            {
                if (!string.IsNullOrEmpty(sItem) && lstLabel.Count(n => n == sItem) > 1)
                {
                    if (!dicoverlap_Item.ContainsKey(iRow))
                        dicoverlap_Item.Add(iRow, string.Format("TagCategoryID : {0}", sItem));
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
            List<EDB_InstrumentTagClassMapping> InstrumentTagClassMappings = new List<EDB_InstrumentTagClassMapping>();

            for (int rowIndex = 0; rowIndex < fpSpreadData.ActiveSheet.Rows.Count; rowIndex++)
            {
                var instrumentIndexAttribute = new EDB_InstrumentTagClassMapping();
                instrumentIndexAttribute.DeleteFlag = false;
                instrumentIndexAttribute.ModifiedDate = DateTime.Now;
                instrumentIndexAttribute.Orders = (rowIndex + 1) * 100;
                instrumentIndexAttribute.InstrumentTagClassMappingID = 0;
                instrumentIndexAttribute.TagCategoryID = Convert.ToInt32(fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.TagCategoryID].Value);
                instrumentIndexAttribute.TagCategoryName = fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.TagCategoryName].Text;
                instrumentIndexAttribute.TagConvTypeID = Convert.ToInt32(fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.TagConvTypeID].Value);
                instrumentIndexAttribute.TagConvTypeName = fpSpreadData.ActiveSheet.Cells[rowIndex, (int)LTColumn.TagConvTypeName].Text;
                InstrumentTagClassMappings.Add(instrumentIndexAttribute);
            }

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                proxy.CreateInstrumentTagClassMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, InstrumentTagClassMappings);
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
