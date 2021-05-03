using System;
using System.Collections.Generic;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread.CellType;

//using EDCS.WF.DSL;

namespace Configuration
{
    public partial class UserControlInstrumentProcessDataOwnership : UserControlBase
    {
        private enum enColumn { InstrumentProcessDataOwnershipID, Name, BackColor, Orders, Last };
        private string[] mColumnName = new string[] { "InstrumentProcessDataOwnershipID", "Name", "BackColor", "Orders" };
        private int[] mColumnWidth = new int[] { 100, 250, 150, 100 };
        private bool[] mColumnLock = new bool[] { true, false, false, true };
        SpreadUtil.eFarpointCellType[] mColumnType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };

        public UserControlInstrumentProcessDataOwnership()
        {
            InitializeComponent();

        }
        public override void LoadData()
        {
            setSpread();
        }

        public void InitSpreadSheet()
        {
            SpreadUtil.InitSpread(fpSpreadOwnership, mColumnWidth, mColumnName, mColumnLock, null, null, mColumnType, null, null, Color.LightGray, true, true);

            var colorPicker = new ColorPickerCellType();
            this.fpSpreadOwnership.ActiveSheet.Columns[(int)enColumn.BackColor].CellType = colorPicker;
        }

        private void setSpread()
        {
            InitSpreadSheet();

            // Setting columns
            fpSpreadOwnership.ActiveSheet.Columns[(int)enColumn.InstrumentProcessDataOwnershipID].Visible = false;
            fpSpreadOwnership.ActiveSheet.Columns[(int)enColumn.Orders].Visible = false;

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                // Load to spread sheet 'Instrument Process Data Ownership'
                var InstrumentProcessDataOwnerships = proxy.ReadInstrumentProcessDataOwnership(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                int iRowCount = 0;
                foreach (var ownership in InstrumentProcessDataOwnerships)
                {
                    iRowCount = fpSpreadOwnership.ActiveSheet.Rows.Count++;

                    fpSpreadOwnership.ActiveSheet.Cells[iRowCount, (int)enColumn.InstrumentProcessDataOwnershipID].Value = ownership.InstrumentProcessDataOwnershipID;
                    fpSpreadOwnership.ActiveSheet.Cells[iRowCount, (int)enColumn.Name].Value = ownership.Name;
                    if (ownership.BackColorArgb != null)
                        fpSpreadOwnership.ActiveSheet.Cells[iRowCount, (int)enColumn.BackColor].Value = Color.FromArgb(ownership.BackColorArgb.Value);
                    fpSpreadOwnership.ActiveSheet.Cells[iRowCount, (int)enColumn.Orders].Text = ownership.Orders.ToString();
                }
            }
        }

        public override void SaveData()
        {
            try
            {
                string sMessage = string.Empty;
                StringBuilder sbErrorMessage = new StringBuilder();

                if (fpSpreadOwnership.ActiveSheet.RowCount > 0)
                {
                    //EdcsDac db = new EdcsDac();
                    //var InstrumentProcessDataOwnerships = db.ReadInstrumentProcessDataOwnership(false);

                    bool bResult = SaveOwnership(ref sMessage);

                    if (bResult == false)
                    {
                        MessageBox.Show(sMessage.ToString());
                    }
                    else
                    {
                        setSpread();
                        MessageBox.Show("Saved successfully!", "Save", MessageBoxButtons.OK, MessageBoxIcon.Information);
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

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sMessage"></param>
        /// <returns></returns>
        public bool SaveOwnership(ref string sMessage)
        {
            sMessage = string.Empty;
            bool bResult = false;
            bool bSaveFlag = true;
            List<EDB_InstrumentProcessDataOwnership> InstrumentProcessDataOwnerships = new List<EDB_InstrumentProcessDataOwnership>();

            for (int iRowCount = 0; iRowCount < fpSpreadOwnership.ActiveSheet.RowCount; iRowCount++)
            {
                EDB_InstrumentProcessDataOwnership InstrumentProcessDataOwnership = new EDB_InstrumentProcessDataOwnership();
                InstrumentProcessDataOwnership.InstrumentProcessDataOwnershipID = Convert.ToInt32(fpSpreadOwnership.ActiveSheet.Cells[iRowCount, (int)enColumn.InstrumentProcessDataOwnershipID].Value);
                InstrumentProcessDataOwnership.Name = fpSpreadOwnership.ActiveSheet.Cells[iRowCount, (int)enColumn.Name].Text;
                InstrumentProcessDataOwnership.DeleteFlag = false;
                InstrumentProcessDataOwnership.ModifiedDate = DateTime.Now;
                InstrumentProcessDataOwnership.Orders = Convert.ToInt32(fpSpreadOwnership.ActiveSheet.Cells[iRowCount, (int)enColumn.Orders].Value);
                if (fpSpreadOwnership.ActiveSheet.Cells[iRowCount, (int)enColumn.BackColor].Value != null)
                {
                    Color backColor = (Color)fpSpreadOwnership.ActiveSheet.Cells[iRowCount, (int)enColumn.BackColor].Value;
                    InstrumentProcessDataOwnership.BackColorArgb = backColor.ToArgb();
                }
                InstrumentProcessDataOwnerships.Add(InstrumentProcessDataOwnership);

            }//fpSpreadRole Row count


            if (bSaveFlag)
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    proxy.CreateInstrumentProcessDataOwnership(EDCS.WF.BSL.Variable.SharedVariable.IsIE, InstrumentProcessDataOwnerships);
                }

                bResult = true;
            }
            else
            {
                sMessage = string.Format("{0} : Column 'Name' is empty");
            }

            return bResult;
        }

    }
}
