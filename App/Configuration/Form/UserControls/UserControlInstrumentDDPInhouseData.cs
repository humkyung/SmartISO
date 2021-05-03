using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;
using EDCS.WF.BSL;
using EDCS.WF.DSL;
using FarPoint.Win.Spread;

using Telerik.WinControls.UI.Data;
using EDCS.WF.DSL.Entities;

namespace Configuration
{
    public partial class UserControlInstrumentDDPInhouseData : UserControlBase
    {
        private enum LTColumn
        {
            BodyType,
            OperCode,
            Size,
            Rating,
            ConType,
            Vendor,
            Project,
            P2,
            P3,
            P4,
            P5,
            P6,
            P7,
            P8,
            P9,
            Y5,
            Y6,
            Y7,
            Y8,
            Y9,
            Y10,
            Y11,
            Y12,
            Y13,
            Y14,
            Y15,
        };

        private string[] mColumnName = new string[]
                {
                    "BodyType",
                    "OperCode",
                    "Size",
                    "Rating",
                    "ConType",
                    "Vendor",
                    "Project",
                    "P2",
                    "P3",
                    "P4",
                    "P5",
                    "P6",
                    "P7",
                    "P8",
                    "P9",
                    "Y5",
                    "Y6",
                    "Y7",
                    "Y8",
                    "Y9",
                    "Y10",
                    "Y11",
                    "Y12",
                    "Y13",
                    "Y14",
                    "Y15",
                };

        private int[] marColumnWidths = new int[] { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 };

        public UserControlInstrumentDDPInhouseData()
        {
            InitializeComponent();
        }

        public override void LoadData()
        {
            radDropDownList3DModel.Items.Clear();
            string[] arItems = Type3D.GetItem3D().ToArray();
            foreach (string item in arItems)
            {
                radDropDownList3DModel.Items.Add(item);
            }
            radDropDownList3DModel.SelectedIndex = 2;
        }

        private void InitSpread()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var InhouseDDPDynamics = proxy.ReadInhouseDDPDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE).Where(w => w.Type3D == radDropDownList3DModel.Text).ToList();
                LoadSpead(InhouseDDPDynamics);
            }
        }

        public override void SaveData()
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    string sMessage = string.Empty;
                    List<EDB_InhouseDDPDynamic> InhouseDDPDynamics = ConvertToListEDB(ref fpSpreadData);
                    proxy.CreateInhouseDDPDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, InhouseDDPDynamics);
                    MessageBox.Show(this, "Saved successfully!", this.Text);
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

        // List<> => FPSpread
        private void LoadSpead(List<EntityEDB_InhouseDDPDynamic> InhouseDDPDynamics)
        {
            fpSpreadData.Sheets.Clear();
            if (InhouseDDPDynamics.Count == 0)
            {
                fpSpreadData.Sheets.Add(new SheetView());
                SpreadUtil.InitSpread(fpSpreadData, marColumnWidths, mColumnName, null, null, null, null, null, null, Color.LightGray, true, true);
            }

            string instrumentType = "";
            // Type 별 Sheet 생성
            foreach (var InhouseInstrumentType in InhouseDDPDynamics.GroupBy(g => g.InstrumentType))
            {
                var InhouseInstrumentTypes = InhouseInstrumentType.ToList();
                if (InhouseInstrumentTypes.Count > 0)
                {
                    instrumentType = InhouseInstrumentTypes[0].InstrumentType;
                    fpSpreadData.Sheets.Add(new SheetView());
                    SheetView sheet = fpSpreadData.Sheets[fpSpreadData.Sheets.Count - 1];
                    fpSpreadData.ActiveSheet = sheet;
                    sheet.SheetName = instrumentType;
                    SpreadUtil.InitSpread(fpSpreadData, marColumnWidths, mColumnName, null, null, null, null, null, null, Color.LightGray, true, true);

                    int rowIndex = 0;
                    sheet.RowCount = InhouseInstrumentTypes.Count;
                    foreach (var row in InhouseInstrumentTypes)
                    {
                        sheet.Cells[rowIndex, (int)LTColumn.BodyType].Text = row.BodyType;
                        sheet.Cells[rowIndex, (int)LTColumn.OperCode].Text = row.OperCode;
                        sheet.Cells[rowIndex, (int)LTColumn.Size].Value = row.Size;
                        sheet.Cells[rowIndex, (int)LTColumn.Rating].Text = row.Rating;
                        sheet.Cells[rowIndex, (int)LTColumn.ConType].Text = row.ConType;
                        sheet.Cells[rowIndex, (int)LTColumn.Vendor].Text = row.Vendor;
                        sheet.Cells[rowIndex, (int)LTColumn.Project].Text = row.Project;
                        sheet.Cells[rowIndex, (int)LTColumn.P2].Text = row.P2;
                        sheet.Cells[rowIndex, (int)LTColumn.P3].Text = row.P3;
                        sheet.Cells[rowIndex, (int)LTColumn.P4].Text = row.P4;
                        sheet.Cells[rowIndex, (int)LTColumn.P5].Text = row.P5;
                        sheet.Cells[rowIndex, (int)LTColumn.P6].Text = row.P6;
                        sheet.Cells[rowIndex, (int)LTColumn.P7].Text = row.P7;
                        sheet.Cells[rowIndex, (int)LTColumn.P8].Text = row.P8;
                        sheet.Cells[rowIndex, (int)LTColumn.P9].Text = row.P9;
                        sheet.Cells[rowIndex, (int)LTColumn.Y5].Text = row.Y5;
                        sheet.Cells[rowIndex, (int)LTColumn.Y6].Text = row.Y6;
                        sheet.Cells[rowIndex, (int)LTColumn.Y7].Text = row.Y7;
                        sheet.Cells[rowIndex, (int)LTColumn.Y8].Text = row.Y8;
                        sheet.Cells[rowIndex, (int)LTColumn.Y9].Text = row.Y9;
                        sheet.Cells[rowIndex, (int)LTColumn.Y10].Text = row.Y10;
                        sheet.Cells[rowIndex, (int)LTColumn.Y11].Text = row.Y11;
                        sheet.Cells[rowIndex, (int)LTColumn.Y12].Text = row.Y12;
                        sheet.Cells[rowIndex, (int)LTColumn.Y13].Text = row.Y13;
                        sheet.Cells[rowIndex, (int)LTColumn.Y14].Text = row.Y14;
                        sheet.Cells[rowIndex, (int)LTColumn.Y15].Text = row.Y15;
                        ++rowIndex;
                    }
                }
            }
        }

        // FPSpread => List<EDB_InhouseDDPDynamic>
        private List<EDB_InhouseDDPDynamic> ConvertToListEDB(ref FpSpread fpSpread)
        {
            List<EDB_InhouseDDPDynamic> InhouseDDPDynamics = new List<EDB_InhouseDDPDynamic>();
            foreach (SheetView sheet in fpSpread.Sheets)
            {
                string instrumentType = sheet.SheetName;
                for (int rowIndex = 0; rowIndex < sheet.NonEmptyRowCount; rowIndex++)
                {
                    EDB_InhouseDDPDynamic InhouseDDPDynamic = new EDB_InhouseDDPDynamic
                    {
                        Type3D = radDropDownList3DModel.Text,
                        InstrumentType = instrumentType,
                        BodyType = sheet.Cells[rowIndex, (int)LTColumn.BodyType].Text,
                        OperCode = sheet.Cells[rowIndex, (int)LTColumn.OperCode].Text,
                        Size = sheet.Cells[rowIndex, (int)LTColumn.Size].Text,
                        Rating = sheet.Cells[rowIndex, (int)LTColumn.Rating].Text,
                        ConType = sheet.Cells[rowIndex, (int)LTColumn.ConType].Text,
                        Vendor = sheet.Cells[rowIndex, (int)LTColumn.Vendor].Text,
                        Project = sheet.Cells[rowIndex, (int)LTColumn.Project].Text,
                        DeleteFlag = false,
                        ModifiedDate = DateTime.Now,
                        P2 = sheet.Cells[rowIndex, (int)LTColumn.P2].Text,
                        P3 = sheet.Cells[rowIndex, (int)LTColumn.P3].Text,
                        P4 = sheet.Cells[rowIndex, (int)LTColumn.P4].Text,
                        P5 = sheet.Cells[rowIndex, (int)LTColumn.P5].Text,
                        P6 = sheet.Cells[rowIndex, (int)LTColumn.P6].Text,
                        P7 = sheet.Cells[rowIndex, (int)LTColumn.P7].Text,
                        P8 = sheet.Cells[rowIndex, (int)LTColumn.P8].Text,
                        P9 = sheet.Cells[rowIndex, (int)LTColumn.P9].Text,
                        Y5 = sheet.Cells[rowIndex, (int)LTColumn.Y5].Text,
                        Y6 = sheet.Cells[rowIndex, (int)LTColumn.Y6].Text,
                        Y7 = sheet.Cells[rowIndex, (int)LTColumn.Y7].Text,
                        Y8 = sheet.Cells[rowIndex, (int)LTColumn.Y8].Text,
                        Y9 = sheet.Cells[rowIndex, (int)LTColumn.Y9].Text,
                        Y10 = sheet.Cells[rowIndex, (int)LTColumn.Y10].Text,
                        Y11 = sheet.Cells[rowIndex, (int)LTColumn.Y11].Text,
                        Y12 = sheet.Cells[rowIndex, (int)LTColumn.Y12].Text,
                        Y13 = sheet.Cells[rowIndex, (int)LTColumn.Y13].Text,
                        Y14 = sheet.Cells[rowIndex, (int)LTColumn.Y14].Text,
                        Y15 = sheet.Cells[rowIndex, (int)LTColumn.Y15].Text,
                    };
                    InhouseDDPDynamics.Add(InhouseDDPDynamic);
                }
            }

            return InhouseDDPDynamics;
        }

        // FPSpread => List<EDB_InhouseDDPDynamic>
        private List<EntityEDB_InhouseDDPDynamic> ConvertToListEntity(FpSpread fpSpread)
        {
            List<EntityEDB_InhouseDDPDynamic> InhouseDDPDynamics = new List<EntityEDB_InhouseDDPDynamic>();
            foreach (SheetView sheet in fpSpread.Sheets)
            {
                string instrumentType = sheet.SheetName;
                for (int rowIndex = 0; rowIndex < sheet.NonEmptyRowCount; rowIndex++)
                {
                    // 첫번째 Row 는 Header 이므로 SKIP
                    if (rowIndex == 0) continue;

                    EntityEDB_InhouseDDPDynamic InhouseDDPDynamic = new EntityEDB_InhouseDDPDynamic
                    {
                        Type3D = radDropDownList3DModel.Text,
                        InstrumentType = instrumentType,
                        BodyType = sheet.Cells[rowIndex, (int)LTColumn.BodyType].Text,
                        OperCode = sheet.Cells[rowIndex, (int)LTColumn.OperCode].Text,
                        Size = sheet.Cells[rowIndex, (int)LTColumn.Size].Text,
                        Rating = sheet.Cells[rowIndex, (int)LTColumn.Rating].Text,
                        ConType = sheet.Cells[rowIndex, (int)LTColumn.ConType].Text,
                        Vendor = sheet.Cells[rowIndex, (int)LTColumn.Vendor].Text,
                        Project = sheet.Cells[rowIndex, (int)LTColumn.Project].Text,
                        DeleteFlag = false,
                        ModifiedDate = DateTime.Now,
                        P2 = sheet.Cells[rowIndex, (int)LTColumn.P2].Text,
                        P3 = sheet.Cells[rowIndex, (int)LTColumn.P3].Text,
                        P4 = sheet.Cells[rowIndex, (int)LTColumn.P4].Text,
                        P5 = sheet.Cells[rowIndex, (int)LTColumn.P5].Text,
                        P6 = sheet.Cells[rowIndex, (int)LTColumn.P6].Text,
                        P7 = sheet.Cells[rowIndex, (int)LTColumn.P7].Text,
                        P8 = sheet.Cells[rowIndex, (int)LTColumn.P8].Text,
                        P9 = sheet.Cells[rowIndex, (int)LTColumn.P9].Text,
                        Y5 = sheet.Cells[rowIndex, (int)LTColumn.Y5].Text,
                        Y6 = sheet.Cells[rowIndex, (int)LTColumn.Y6].Text,
                        Y7 = sheet.Cells[rowIndex, (int)LTColumn.Y7].Text,
                        Y8 = sheet.Cells[rowIndex, (int)LTColumn.Y8].Text,
                        Y9 = sheet.Cells[rowIndex, (int)LTColumn.Y9].Text,
                        Y10 = sheet.Cells[rowIndex, (int)LTColumn.Y10].Text,
                        Y11 = sheet.Cells[rowIndex, (int)LTColumn.Y11].Text,
                        Y12 = sheet.Cells[rowIndex, (int)LTColumn.Y12].Text,
                        Y13 = sheet.Cells[rowIndex, (int)LTColumn.Y13].Text,
                        Y14 = sheet.Cells[rowIndex, (int)LTColumn.Y14].Text,
                        Y15 = sheet.Cells[rowIndex, (int)LTColumn.Y15].Text,
                    };
                    InhouseDDPDynamics.Add(InhouseDDPDynamic);
                }
            }

            return InhouseDDPDynamics;
        }

        private void ImportExcel(string filePath)
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                fpSpreadExcel.Sheets.Clear();
                fpSpreadExcel.OpenExcel(filePath);

                List<EntityEDB_InhouseDDPDynamic> originalInhouseDDPDynamics = proxy.ReadInhouseDDPDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                List<EntityEDB_InhouseDDPDynamic> excelInhouseDDPDynamics = ConvertToListEntity(fpSpreadExcel);

                for (int index = 0; index < excelInhouseDDPDynamics.Count; index++)
                {
                    var excelInhouseDDPDynamic = excelInhouseDDPDynamics[index];
                    string instrumentType = excelInhouseDDPDynamic.InstrumentType;
                    string bodyType = excelInhouseDDPDynamic.BodyType;
                    string operCode = excelInhouseDDPDynamic.OperCode;
                    string size = excelInhouseDDPDynamic.Size;
                    string rating = excelInhouseDDPDynamic.Rating;

                    var whereInhouseDDPDynamics = originalInhouseDDPDynamics.Where(w => w.InstrumentType == instrumentType
                        && w.BodyType == bodyType
                        && w.OperCode == operCode
                        && w.Size == size
                        && w.Rating == rating).ToList();

                    // 기존 DDP 존재
                    if (whereInhouseDDPDynamics.Count > 0)
                    {
                        whereInhouseDDPDynamics[0].P2 = excelInhouseDDPDynamic.P2;
                        whereInhouseDDPDynamics[0].P3 = excelInhouseDDPDynamic.P3;
                        whereInhouseDDPDynamics[0].P4 = excelInhouseDDPDynamic.P4;
                        whereInhouseDDPDynamics[0].P5 = excelInhouseDDPDynamic.P5;
                        whereInhouseDDPDynamics[0].P6 = excelInhouseDDPDynamic.P6;
                        whereInhouseDDPDynamics[0].P7 = excelInhouseDDPDynamic.P7;
                        whereInhouseDDPDynamics[0].P8 = excelInhouseDDPDynamic.P8;
                        whereInhouseDDPDynamics[0].P9 = excelInhouseDDPDynamic.P9;
                        whereInhouseDDPDynamics[0].Y5 = excelInhouseDDPDynamic.Y5;
                        whereInhouseDDPDynamics[0].Y6 = excelInhouseDDPDynamic.Y6;
                        whereInhouseDDPDynamics[0].Y7 = excelInhouseDDPDynamic.Y7;
                        whereInhouseDDPDynamics[0].Y8 = excelInhouseDDPDynamic.Y8;
                        whereInhouseDDPDynamics[0].Y9 = excelInhouseDDPDynamic.Y9;
                        whereInhouseDDPDynamics[0].Y10 = excelInhouseDDPDynamic.Y10;
                        whereInhouseDDPDynamics[0].Y11 = excelInhouseDDPDynamic.Y11;
                        whereInhouseDDPDynamics[0].Y12 = excelInhouseDDPDynamic.Y12;
                        whereInhouseDDPDynamics[0].Y13 = excelInhouseDDPDynamic.Y13;
                        whereInhouseDDPDynamics[0].Y14 = excelInhouseDDPDynamic.Y14;
                        whereInhouseDDPDynamics[0].Y15 = excelInhouseDDPDynamic.Y15;
                    }
                    else
                    {
                        originalInhouseDDPDynamics.Add(excelInhouseDDPDynamic);
                    }
                }

                LoadSpead(originalInhouseDDPDynamics);
            }
        }

        private void radButtonImport_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Title = "Open Excel Data File";
            ofd.Filter = "Excel Data File(*.xls, *.xlsx)|*.xls; *.xlsx";
            ofd.FilterIndex = 1;
            if (DialogResult.OK == ofd.ShowDialog())
            {
                string filePath = ofd.FileName;
                ImportExcel(filePath);
            }

        }

        private void radDropDownListType3D_SelectedIndexChanged(object sender, PositionChangedEventArgs e)
        {
            InitSpread();
        }
    }
}
