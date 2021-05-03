using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using EDCS.WF.BSL.Helpers;
using EDCS.WF.BSL.Variable;
using EDCS.WF.DSL;
using Telerik.WinControls;
using Telerik.WinControls.UI;

namespace Configuration
{
    public partial class UserControlInstrumentDDPMapping : UserControlBase
    {
        string themeName = "Windows8";

        int ListTypeInstrumentProcessData = 3;
        int ListTypeInstrument3DDimension = 18;

        string[] EXCEL_COLUMNS = new string[] { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "AA", "AB", "AC", "AD", "AE", "AF", "AG", "AH", "AI", "AJ", "AK", "AL", "AM", "AN", "AO", "AP", "AQ", "AR", "AS", "AT", "AU", "AV", "AW", "AX", "AY", "AZ", "BA", "BB", "BC", "BD", "BE", "BF", "BG", "BH", "BI", "BJ", "BK", "BL", "BM", "BN", "BO", "BP", "BQ", "BR", "BS", "BT", "BU", "BV", "BW", "BX", "BY", "BZ" };

        public class ExcelColumns
        {
            public int Index { get; set; }
            public string Name { get; set; }
        }

        public UserControlInstrumentDDPMapping()
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
            //this.MethodTheme();
            this.MethodDesignForm();

            this.radDropDownCataType.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownCataType_SelectedIndexChanged);
            this.radDropDownCataType.Items.Clear();
            this.radDropDownCataType.Items.Add("Body");
            this.radDropDownCataType.Items.Add("Actuator");
            this.radDropDownCataType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownCataType_SelectedIndexChanged);
            this.radDropDownCataType.SelectedIndex = 0;
        }
        public override void SaveData()
        {
            this.MethodSaveShareRadGridView();
        }

        private void MethodBindingRadGridView()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                this.radGridView1.BeginUpdate();
                this.radGridView1.Rows.Clear();

                int ddpSheetID = (int)this.radDropDownListSheet.SelectedValue;
                var InstrumentDDPSheets = proxy.ReadInstrumentDDPMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, ddpSheetID);
                // Rows Setting
                foreach (var InstrumentDDPSheet in InstrumentDDPSheets)
                {
                    this.radGridView1.Rows.Add(false, InstrumentDDPSheet.InstrumentDDPMappingID, Convert.ToInt32(InstrumentDDPSheet.Label), InstrumentDDPSheet.MappingColumn, InstrumentDDPSheet.CustomValue);
                }

                this.radGridView1.EndUpdate();
            }
        }

        private void MethodSettingRadGridView()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                this.radGridView1.BeginUpdate();
                this.radGridView1.Rows.Clear();
                this.radGridView1.Columns.Clear();

                // Columns Setting
                this.radGridView1.Columns.Add(new GridViewCheckBoxColumn()
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
                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "InstrumentDDPMappingID",
                    IsVisible = false,
                });
                List<ExcelColumns> excelColumns = new List<ExcelColumns>();
                for (int index = 0; index < EXCEL_COLUMNS.Length; index++)
                {
                    excelColumns.Add(new ExcelColumns() { Index = index, Name = EXCEL_COLUMNS[index] });
                }

                this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "Label",
                    HeaderText = "ExcelColumn",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    DataSource = excelColumns,
                    DisplayMember = "Name",
                    ValueMember = "Index",
                });
                var columnDynamicDefaults = proxy.ReadColumnDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE).Where(w => w.ListTypeID == ListTypeInstrument3DDimension).ToList();
                if (radDropDownListBody.SelectedValue != null)
                {
                    var instrumentCataTypeColumns = proxy.ReadInstrument3DCataTypeColumn(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, (int)radDropDownListBody.SelectedValue);
                    foreach (var instrumentCataTypeColumn in instrumentCataTypeColumns)
                    {
                        columnDynamicDefaults.Add(new EntityEDB_ColumnDynamicDefault() { Label = instrumentCataTypeColumn.Label, LabelView = instrumentCataTypeColumn.LabelView });
                    }
                }
                columnDynamicDefaults.Add(new EntityEDB_ColumnDynamicDefault() { Label = "Custom" });
                this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                {
                    Name = "MappingColumn",
                    HeaderText = "EDB Column",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    DataSource = columnDynamicDefaults,
                    DisplayMember = "Label",
                    ValueMember = "Label",
                });
                this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                {
                    Name = "CustomValue",
                    HeaderText = "Custom Value",
                    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    TextAlignment = ContentAlignment.MiddleCenter,
                    //
                    MaxLength = 100,
                });

                //this.MethodBindingRadGridView();

                this.radGridView1.EndUpdate();
            }
        }

        private void MethodDesignForm()
        {
            // radGridView1
            this.radGridView1.AllowAutoSizeColumns = false;
            this.radGridView1.AllowColumnChooser = false;
            this.radGridView1.AllowColumnHeaderContextMenu = false;
            this.radGridView1.AllowColumnReorder = false;
            this.radGridView1.AllowMultiColumnSorting = false;
            this.radGridView1.AllowRowResize = false;
            this.radGridView1.AutoGenerateColumns = false;
            this.radGridView1.AutoSizeColumnsMode = GridViewAutoSizeColumnsMode.Fill;
            this.radGridView1.EnableHotTracking = false;
            this.radGridView1.ShowCellErrors = false;
            this.radGridView1.ShowFilteringRow = false;
            this.radGridView1.ShowNoDataText = false;
            this.radGridView1.ShowRowErrors = false;
            this.radGridView1.AllowRowReorder = true;
            this.radGridView1.TableElement.TableHeaderHeight = 20;
            this.radGridView1.TableElement.RowHeight = 21;
            this.radGridView1.MasterTemplate.ShowRowHeaderColumn = false;
            this.radGridView1.MasterTemplate.ShowHeaderCellButtons = false;
            this.radGridView1.AllowAddNewRow = false;
            this.radGridView1.Font = new Font("Tahoma", (float)8.25, FontStyle.Regular);
            this.radGridView1.SelectionMode = GridViewSelectionMode.FullRowSelect;
            this.radGridView1.MultiSelect = true;
            this.radGridView1.EnableGrouping = false;
            this.radGridView1.AllowCellContextMenu = false;

            this.radDropDownCataType.ListElement.Font = new Font("Tahoma", (float)8.25, FontStyle.Regular);

        }

        private void MethodSaveShareRadGridView()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                //for (int i = 0; i < this.radGridView1.RowCount; i++)
                //{
                //    for (int j = 1; j < this.radGridView1.ColumnCount; j++) // j가 1부터 시작되는 이유 : 첫번째 Column 제외 (ID값은 추가시 자동으로 부여되기 때문에 없어야 함)
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

                // Cata Type            
                var InstrumentDDPMappings = new List<EDB_InstrumentDDPMapping>();

                int ddpSheetID = (int)radDropDownListSheet.SelectedValue;
                foreach (var row in this.radGridView1.Rows.OrderBy(ob => ob.Index))
                {
                    var InstrumentDDPMapping = new EDB_InstrumentDDPMapping();

                    InstrumentDDPMapping.InstrumentDDPMappingID = 0;
                    InstrumentDDPMapping.DeleteFlag = false;
                    InstrumentDDPMapping.Orders = (row.Index + 1) * 100;
                    InstrumentDDPMapping.ModifiedDate = DateTime.Now;
                    InstrumentDDPMapping.InstrumentDDPSheetID = ddpSheetID;
                    InstrumentDDPMapping.MappingColumn = row.Cells["MappingColumn"].Value != null ? row.Cells["MappingColumn"].Value.ToString().Trim() : null;
                    InstrumentDDPMapping.Label = row.Cells["Label"].Value != null ? row.Cells["Label"].Value.ToString().Trim() : null;
                    InstrumentDDPMapping.CustomValue = row.Cells["CustomValue"].Value != null ? row.Cells["CustomValue"].Value.ToString().Trim() : null;

                    InstrumentDDPMappings.Add(InstrumentDDPMapping);
                }

                proxy.CreateInstrumentDDPMapping(EDCS.WF.BSL.Variable.SharedVariable.IsIE, InstrumentDDPMappings, ddpSheetID);
            }
        }

        private void radDropDownCataType_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            //if (radDropDownListBody.Items.Count > 0) radDropDownListBody.SelectedIndex = 0;
            try
            {

                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    string cataTypeName = this.radDropDownCataType.Text;

                    radDropDownListBody.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListBody_SelectedIndexChanged);
                    radDropDownListBody.Items.Clear();
                    radDropDownListBody.ValueMember = "Instrument3DCataTypeID";
                    radDropDownListBody.DisplayMember = "Name";
                    radDropDownListBody.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListBody_SelectedIndexChanged);
                    radDropDownListBody.DataSource = proxy.ReadInstrument3DCataType(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, cataTypeName);
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

        private void radDropDownListBody_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            try
            {
                var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
                using (proxy as IDisposable)
                {
                    this.MethodSettingRadGridView();

                    int cataTypeID = (int)radDropDownListBody.SelectedValue;

                    radDropDownListSheet.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListSheet_SelectedIndexChanged);
                    radDropDownListSheet.Items.Clear();
                    radDropDownListSheet.ValueMember = "InstrumentDDPSheetID";
                    radDropDownListSheet.DisplayMember = "Name";
                    radDropDownListSheet.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListSheet_SelectedIndexChanged);
                    radDropDownListSheet.DataSource = proxy.ReadInstrumentDDPSheet(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, cataTypeID).Where(w => w.DeleteFlag == false).ToList();
                    //if (radDropDownListSheet.Items.Count > 0) radDropDownListSheet.SelectedIndex = 0;
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

        private void radDropDownListSheet_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            this.MethodBindingRadGridView();
        }

        private void radButtonInsertRow_Click(object sender, EventArgs e)
        {
            GridViewRowInfo oGridRow = this.radGridView1.Rows.AddNew();
        }

        private void radButtonRemoveRow_Click(object sender, EventArgs e)
        {
            for (int i = this.radGridView1.Rows.Count - 1; i > -1; i--)
            {
                if (this.radGridView1.Rows[i].Cells["DeleteCheck"].Value != null && (bool)this.radGridView1.Rows[i].Cells["DeleteCheck"].Value == true)
                {
                    this.radGridView1.Rows.RemoveAt(i);
                }
            }
        }

    }
}
