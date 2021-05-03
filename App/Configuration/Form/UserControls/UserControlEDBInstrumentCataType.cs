using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL.Helpers;
using EDCS.WF.BSL;
using EDCS.WF.BSL.Variable;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;

using Telerik.WinControls;
using Telerik.WinControls.UI.Data;

namespace Configuration
{
    public partial class UserControlEDBInstrumentCataType : UserControlBase
    {
        string themeName = "Windows8";

        int ListTypeInstrumentProcessData = 3;
        int ListTypeInstrument3DDimension = 18;

        private enum menColumn { Select, Instrument3DCataTypeID, Name, Last };
        private string[] marColumnName = { " ", "Instrument3DCataTypeID", "Name" };
        private int[] marColumnWidth = { 40, 0, 200 };
        private bool[] mColumnLock = new bool[] { false, true, false };
        private SpreadUtil.eFarpointCellType[] mColumnCellType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };


        private enum menColumnColumn { Select, Instrument3DCataTypeColumnID, LabelView, Last };
        private string[] marColumnColumnName = { " ", "Instrument3DCataTypeColumnID", "LabelView" };
        private int[] marColumnColumnWidth = { 40, 0, 200 };
        private bool[] mColumnColumnLock = new bool[] { false, true, false };
        private SpreadUtil.eFarpointCellType[] mColumnColumnCellType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };


        List<EntityEDB_Instrument3DCataTypeColumn> _originalCataTypeColumns = new List<EntityEDB_Instrument3DCataTypeColumn>();

        public UserControlEDBInstrumentCataType()
        {
            try
            {
                this.InitializeComponent();

                //this.Dock = DockStyle.Fill;
            }
            catch (Exception exception)
            {
                new ErrorHandling(this, exception, this._entityEDBInformation.Employee, this._entityEDBInformation.Project, this._entityEDBInformation.ListType).ErrorMsgDatabaseSave();
            }
        }

        public override void LoadData()
        {
            //this.radDropDownCataType.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownCataType_SelectedIndexChanged);

            this.MethodDesignForm();

            this.radDropDownCataType.Items.Clear();
            this.radDropDownCataType.Items.Add("Body");
            this.radDropDownCataType.Items.Add("Actuator");
            this.radDropDownCataType.SelectedIndex = 0;

            this.MethodSettingRadGridView();
            this.MethodSettingRadGridViewAttributes();
        }

        public override void SaveData()
        {
            //this.MethodSaveShareRadGridView();
        }

        private void MethodBindingRadGridView()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                string cataTypeName = this.radDropDownCataType.Text;

                var instrument3DBodyTypes = proxy.ReadInstrument3DCataType(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, cataTypeName)
                    .ToList();

                this.fpSpread1.ActiveSheet.RowCount = instrument3DBodyTypes.Count;

                // Rows Setting
                for (int rowIndex = 0; rowIndex < instrument3DBodyTypes.Count; rowIndex++)
                {
                    var instrument3DBodyType = instrument3DBodyTypes[rowIndex];
                    this.fpSpread1.ActiveSheet.Cells[rowIndex, (int)menColumn.Select].Value = false;
                    this.fpSpread1.ActiveSheet.Cells[rowIndex, (int)menColumn.Instrument3DCataTypeID].Value = instrument3DBodyType.Instrument3DCataTypeID;
                    this.fpSpread1.ActiveSheet.Cells[rowIndex, (int)menColumn.Name].Value = instrument3DBodyType.Name;
                }
            }
        }

        //private bool SaveChangesColumns()
        //{
        //    bool changed = false;
        //    for (int rowIndex = 0; rowIndex < this.radGridView2.Rows.Count; rowIndex++)
        //    {
        //        int Instrument3DCataTypeColumnID = Convert.ToInt32(this.radGridView2.Rows[rowIndex].Cells["Instrument3DCataTypeColumnID"].Value);
        //        var currentCataTypeColumn = this._originalCataTypeColumns.Where(w => w.Instrument3DCataTypeColumnID == Instrument3DCataTypeColumnID).ToList();
        //        if (currentCataTypeColumn.Count > 0)
        //        {
        //            if (currentCataTypeColumn.First().LabelView != this.radGridView2.Rows[rowIndex].Cells["LabelView"].Value.ToString())
        //            {
        //                changed = true;
        //                break;
        //            }
        //        }
        //        else
        //        {
        //            changed = true;
        //            break;
        //        }
        //    }
        //    return changed;
        //}

        private void MethodBindingRadGridViewAttributes()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                object value = this.fpSpread1.ActiveSheet.Cells[this.fpSpread1.ActiveSheet.ActiveRowIndex, (int)menColumn.Instrument3DCataTypeID].Value;
                if (value == null)
                {
                    this.fpSpread2.ActiveSheet.RowCount = 0;
                    return;
                }
                int cataTypeID = Convert.ToInt32(value);

                var instrument3DCataTypeColumns = proxy.ReadInstrument3DCataTypeColumn(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, cataTypeID)
                    //.Where(w => w.VisibleFlag.Equals(true))
                    .ToList();
                _originalCataTypeColumns = instrument3DCataTypeColumns;

                this.fpSpread2.ActiveSheet.RowCount = instrument3DCataTypeColumns.Count;
                // Rows Setting
                for (int rowIndex = 0; rowIndex < instrument3DCataTypeColumns.Count; rowIndex++)
                {
                    var instrument3DCataTypeColumn = instrument3DCataTypeColumns[rowIndex];
                    this.fpSpread2.ActiveSheet.Cells[rowIndex, (int)menColumnColumn.Select].Value = false;
                    this.fpSpread2.ActiveSheet.Cells[rowIndex, (int)menColumnColumn.Instrument3DCataTypeColumnID].Value = instrument3DCataTypeColumn.Instrument3DCataTypeColumnID;
                    this.fpSpread2.ActiveSheet.Cells[rowIndex, (int)menColumnColumn.LabelView].Value = instrument3DCataTypeColumn.LabelView;
                }
            }
        }

        private void MethodSettingRadGridView()
        {
            SpreadUtil.InitSpread(fpSpread1, marColumnWidth, marColumnName, mColumnLock, null, null, mColumnCellType, null, null, Color.LightGray, true, true);

            this.MethodSettingDropDownListVisible();

            this.MethodBindingRadGridView();
        }

        private void MethodSettingRadGridViewAttributes()
        {
            SpreadUtil.InitSpread(fpSpread2, marColumnColumnWidth, marColumnColumnName, mColumnColumnLock, null, null, mColumnColumnCellType, null, null, Color.LightGray, true, true);
        }

        private void MethodDesignForm()
        {

            this.radDropDownCataType.ListElement.Font = new Font("Tahoma", (float)8.25, FontStyle.Regular);

        }
        // DDP File 을 서버에 업로드한다.
        public bool MethodUploadDDPFile(string ddpFilePath, string cataName)
        {
            string fileName = string.Format("{0}.XLSX", cataName);
            string serverFilePath = Path.Combine(Paths.DDP_ROOT_PATH, fileName);
            //string serverPath = Fundamental.SPIToolInfo.DDP_ROOT_PATH;
            bool uploaded = false;
            try
            {
                if (File.Exists(ddpFilePath))
                {
                    File.Copy(ddpFilePath, serverFilePath, true);
                    uploaded = true;
                }
            }
            catch (Exception ex) { }


            return uploaded;
        }

        private void MethodSaveCataType()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                if (this.radTextBoxDDPFile.Text == "")
                {
                    new CustomRadMessageBoxShowAndTheme("Please select DDP File.").RadMessageBoxShow();
                    return;
                }

                // Cata Type            
                var instrument3DCataTypes = new List<EDB_Instrument3DCataType>();

                for (int i = 0; i < this.fpSpread1.ActiveSheet.RowCount; i++)
                {
                    var instrument3DCataType = new EDB_Instrument3DCataType();

                    instrument3DCataType.Instrument3DCataTypeID = Convert.ToInt32(this.fpSpread1.ActiveSheet.Cells[i, (int)menColumn.Instrument3DCataTypeID].Value);
                    instrument3DCataType.DeleteFlag = false;
                    instrument3DCataType.Orders = (i + 1) * 100;
                    instrument3DCataType.ModifiedDate = DateTime.Now;
                    instrument3DCataType.CataTypeName = this.radDropDownCataType.Text;
                    instrument3DCataType.Name = this.fpSpread1.ActiveSheet.Cells[i, (int)menColumn.Name].Text;

                    instrument3DCataTypes.Add(instrument3DCataType);
                }

                proxy.CreateInstrument3DCataType(EDCS.WF.BSL.Variable.SharedVariable.IsIE, instrument3DCataTypes);

                string cataTypeName = this.radDropDownCataType.Text;
                string typeName = this.fpSpread1.ActiveSheet.Cells[this.fpSpread1.ActiveSheet.ActiveRowIndex, (int)menColumn.Name].Text;
                int CataTypeID = proxy.ReadInstrument3DCataType(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, cataTypeName).Where(w => w.Name.Equals(typeName)).First().Instrument3DCataTypeID;
                // DDP File Upload
                bool uploaded = MethodUploadDDPFile(this.radTextBoxDDPFile.Text, typeName);

                // Save Sheet Name
                if (File.Exists(this.radTextBoxDDPFile.Text))
                {
                    FpSpread fpSpread = new FpSpread();
                    fpSpread.OpenExcel(this.radTextBoxDDPFile.Text);

                    var instrumentDDPSheets = new List<EDB_InstrumentDDPSheet>();
                    for (int sheetIndex = 0; sheetIndex < fpSpread.Sheets.Count; sheetIndex++)
                    {
                        SheetView sheetView = fpSpread.Sheets[sheetIndex];

                        var instrumentDDPSheet = new EDB_InstrumentDDPSheet();

                        instrumentDDPSheet.InstrumentDDPSheetID = 0;
                        instrumentDDPSheet.DeleteFlag = false;
                        instrumentDDPSheet.Orders = (sheetIndex + 1) * 100;
                        instrumentDDPSheet.ModifiedDate = DateTime.Now;
                        instrumentDDPSheet.Name = sheetView.SheetName;
                        instrumentDDPSheet.Instrument3DCataTypeID = CataTypeID;
                        instrumentDDPSheets.Add(instrumentDDPSheet);
                    }

                    proxy.CreateInstrumentDDPSheet(EDCS.WF.BSL.Variable.SharedVariable.IsIE, instrumentDDPSheets, CataTypeID);
                }

                this.MethodBindingRadGridView();
            }
        }

        private void MethodSaveCataTypeColumn()
        {
            //for (int i = 0; i < this.fpSpread1.ActiveSheet.RowCount; i++)
            //{
            //    for (int j = 1; j < this.fpSpread1.ActiveSheet.ColumnCount; j++) // j가 1부터 시작되는 이유 : 첫번째 Column 제외 (ID값은 추가시 자동으로 부여되기 때문에 없어야 함)
            //    {
            //        if (this.radGridView1.Columns[j].IsVisible
            //            && !this.radGridView1.Columns[j].Name.Equals("EquipmentTypeDynamicID")
            //            && !this.radGridView1.Columns[j].Name.Equals("NameKor")
            //            && !this.radGridView1.Columns[j].Name.Equals("NameEng")
            //            && !this.radGridView1.Columns[j].Name.Equals("PWD")
            //            && !this.radGridView1.Columns[j].Name.Equals("PowerEquipmentProjectType")
            //            && !this.radGridView1.Columns[j].Name.Equals("Capacity")
            //            && !this.radGridView1.Columns[j].Name.Equals("PowerEquipmentNational")
            //            && !this.radGridView1.Columns[j].Name.Equals("PrimaryFuelType")
            //            && !this.radGridView1.Columns[j].Name.Equals("CoolingWaterSource")
            //            && !this.radGridView1.Columns[j].Name.Equals("CreationDate")
            //            && !this.radGridView1.Columns[j].Name.Equals("ClosingDate")
            //            && !this.radGridView1.Columns[j].Name.Equals("Type2")
            //            && !this.radGridView1.Columns[j].Name.Equals("Remark")
            //            && !this.radGridView1.Columns[j].Name.Equals("Nde")
            //            && !this.radGridView1.Columns[j].Name.Equals("CommonUnitID")
            //                && !this.radGridView1.Columns[j].Name.Equals("FilePath")
            //            ) //필드 제외
            //        {
            //            if ((this.radGridView1.Rows[i].Cells[j].Value == null || string.IsNullOrWhiteSpace(this.radGridView1.Rows[i].Cells[j].Value.ToString())) && !(this.radGridView1.Rows[i].Cells[j].ColumnInfo is GridViewCheckBoxColumn))
            //            {
            //                //flagSaveWhether = false;
            //                //break;
            //                new CustomRadMessageBoxShowAndTheme("Please fill in all the details without blanks.").RadMessageBoxShow();
            //                return;
            //            }
            //        }
            //    } // end for
            //} // end for


            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                string cataTypeName = this.radDropDownCataType.Text;
                string typeName = this.fpSpread1.ActiveSheet.Cells[this.fpSpread1.ActiveSheet.ActiveRowIndex, (int)menColumn.Name].Text;
                int CataTypeID = proxy.ReadInstrument3DCataType(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, cataTypeName).Where(w => w.Name.Equals(typeName)).First().Instrument3DCataTypeID;

                // Cata Type Column
                var instrument3DCataTypeColumns = new List<EDB_Instrument3DCataTypeColumn>();
                for (int i = 0; i < this.fpSpread2.ActiveSheet.RowCount; i++)
                {
                    var instrument3DCataTypeColumn = new EDB_Instrument3DCataTypeColumn();

                    instrument3DCataTypeColumn.Instrument3DCataTypeID = 0;
                    instrument3DCataTypeColumn.DeleteFlag = false;
                    instrument3DCataTypeColumn.Orders = (i + 1) * 100;
                    instrument3DCataTypeColumn.ModifiedDate = DateTime.Now;
                    instrument3DCataTypeColumn.Label = string.Format("{0}_{1}", cataTypeName.ToUpper(), i);
                    instrument3DCataTypeColumn.LabelView = this.fpSpread2.ActiveSheet.Cells[i, (int)menColumnColumn.LabelView].Text;
                    instrument3DCataTypeColumn.Instrument3DCataTypeID = CataTypeID;
                    instrument3DCataTypeColumns.Add(instrument3DCataTypeColumn);
                }

                proxy.CreateInstrument3DCataTypeColumn(EDCS.WF.BSL.Variable.SharedVariable.IsIE, instrument3DCataTypeColumns, CataTypeID);

                this.MethodBindingRadGridViewAttributes();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="p_type">0 - Body, 1 - Actuator</param>
        private void MethodSettingDropDownListVisible()
        {
            radLabelName.Text = string.Format("{0} Name", radDropDownCataType.Text);
            radLabelAttributes.Text = string.Format("{0} Attribute", radDropDownCataType.Text);
        }


        private void radDropDownCataType_SelectedIndexChanged(object sender, PositionChangedEventArgs e)
        {
            this.MethodBindingRadGridView();
            if (radDropDownCataType.SelectedIndex == 0)
            {
                radLabelName.Text = "Body Name";
                radLabelAttributes.Text = "Body Attributes";
            }
            else
            {
                radLabelName.Text = "Actuator Name";
                radLabelAttributes.Text = "Actuator Attributes";
            }
        }

        private void radButtonInsertRow_Click(object sender, EventArgs e)
        {
            fpSpread1.ActiveSheet.RowCount = fpSpread1.ActiveSheet.RowCount + 1;
        }

        private void radButtonRemoveRow_Click(object sender, EventArgs e)
        {

        }

        private void radButtonInsertRowColumn_Click(object sender, EventArgs e)
        {
            fpSpread2.ActiveSheet.RowCount = fpSpread2.ActiveSheet.RowCount + 1;
        }

        private void radButtonRemoveRowColumn_Click(object sender, EventArgs e)
        {
            if (fpSpread2.ActiveSheet != null)
            {
                for (int i = fpSpread2.ActiveSheet.Rows.Count - 1; i >= 0; i--)
                {
                    string sCheck = fpSpread2.ActiveSheet.Cells[i, (int)menColumn.Select].Text;

                    if (sCheck.ToUpper().Equals("TRUE"))
                    {
                        fpSpread2.ActiveSheet.Rows.Remove(i, 1);
                    }
                }
            }
        }

        private void radButtonOpenExcel_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "Open Excel Data File";
            ofd.Filter = "Excel Data File(*.xls, *.xlsx)|*.xls; *.xlsx";
            ofd.FilterIndex = 1;
            if (DialogResult.OK == ofd.ShowDialog())
            {
                string filePath = ofd.FileName;
                this.radTextBoxDDPFile.Text = filePath;
            }
        }

        private void fpSpread1_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            //if (SaveChangesColumns())
            //{
            //    SaveData();
            //}

            MethodBindingRadGridViewAttributes();

            string cataTypeName = fpSpread1.ActiveSheet.Cells[fpSpread1.ActiveSheet.ActiveRowIndex, (int)menColumn.Name].Text;
            string fileName = string.Format("{0}.XLSX", cataTypeName);
            string serverFilePath = Path.Combine(Paths.DDP_ROOT_PATH, fileName);

            if (File.Exists(serverFilePath))
                radTextBoxDDPFile.Text = fileName;
            else
                radTextBoxDDPFile.Text = "";

        }

        private void radButtonTypeSave_Click(object sender, EventArgs e)
        {
            MethodSaveCataType();
        }

        private void radButtonColumnSave_Click(object sender, EventArgs e)
        {
            MethodSaveCataTypeColumn();
        }

    }
}
