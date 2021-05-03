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
using EDCS.WF.BSL.Enums;

namespace Configuration
{
    public partial class UserControlInstrumentBasicTemplate : UserControlBase
    {
        string themeName = "Windows8";

        int ListTypeInstrumentProcessData = 3;
        int ListTypeInstrument3DDimension = 18;

        private enum menColumn { Select, InstrumentBasicTemplateID, Name, Last };
        private string[] marColumnName = { " ", "InstrumentBasicTemplateID", "Name" };
        private int[] marColumnWidth = { 40, 0, 200 };
        private bool[] mColumnLock = new bool[] { false, true, false };
        private SpreadUtil.eFarpointCellType[] mColumnCellType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };


        private enum menColumnColumn { Select, InstrumentSpecificTemplateID, Name, Last };
        private string[] marColumnColumnName = { " ", "InstrumentSpecificTemplateID", "LabelView" };
        private int[] marColumnColumnWidth = { 40, 0, 200 };
        private bool[] mColumnColumnLock = new bool[] { false, true, false };
        private SpreadUtil.eFarpointCellType[] mColumnColumnCellType = new SpreadUtil.eFarpointCellType[] { SpreadUtil.eFarpointCellType.CheckBox, SpreadUtil.eFarpointCellType.Text, SpreadUtil.eFarpointCellType.Text };


        List<EntityEDB_InstrumentSpecificTemplate> _originalInstrumentSpecificTemplates = new List<EntityEDB_InstrumentSpecificTemplate>();

        public UserControlInstrumentBasicTemplate()
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

            //this.MethodTheme();
            this.MethodDesignForm();

            this.radDropDownListProjectType.Items.Clear();

            Telerik.WinControls.UI.RadListDataItem item = new Telerik.WinControls.UI.RadListDataItem();
            item.Tag = (int)EnumProjectType2.Chemical_Plant;
            item.Text = Enum.GetName(typeof(EnumProjectType2), EnumProjectType2.Chemical_Plant);
            radDropDownListProjectType.Items.Add(item);

            item = new Telerik.WinControls.UI.RadListDataItem();
            item.Tag = (int)EnumProjectType2.Power_Plant;
            item.Text = Enum.GetName(typeof(EnumProjectType2), EnumProjectType2.Power_Plant);
            radDropDownListProjectType.Items.Add(item);

            radDropDownListProjectType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListProjectType_SelectedIndexChanged);
            radDropDownListProjectType.SelectedIndex = 0;

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
                //int projectTypeID = Convert.ToInt32(this.radDropDownListProjectType.Tag);

                var instrumentBasicTemplates = proxy.ReadInstrumentBasicTemplate(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false)
                    .ToList();

                this.fpSpread1.ActiveSheet.RowCount = instrumentBasicTemplates.Count;

                // Rows Setting
                for (int rowIndex = 0; rowIndex < instrumentBasicTemplates.Count; rowIndex++)
                {
                    var instrumentBasicTemplate = instrumentBasicTemplates[rowIndex];
                    this.fpSpread1.ActiveSheet.Cells[rowIndex, (int)menColumn.Select].Value = false;
                    this.fpSpread1.ActiveSheet.Cells[rowIndex, (int)menColumn.InstrumentBasicTemplateID].Value = instrumentBasicTemplate.InstrumentBasicTemplateID;
                    this.fpSpread1.ActiveSheet.Cells[rowIndex, (int)menColumn.Name].Value = instrumentBasicTemplate.NameView;
                }
            }
        }

        private void MethodBindingRadGridViewAttributes()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                object value = this.fpSpread1.ActiveSheet.Cells[this.fpSpread1.ActiveSheet.ActiveRowIndex, (int)menColumn.InstrumentBasicTemplateID].Value;
                if (value == null)
                {
                    this.fpSpread2.ActiveSheet.RowCount = 0;
                    return;
                }
                int projectTypeID = Convert.ToInt32(this.radDropDownListProjectType.SelectedItem.Tag);
                int basicTemplateID = Convert.ToInt32(this.fpSpread1.ActiveSheet.Cells[this.fpSpread1.ActiveSheet.ActiveRowIndex, (int)menColumn.InstrumentBasicTemplateID].Value);

                var instrumentSpecificTemplates = proxy.ReadInstrumentSpecificTemplate(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, basicTemplateID, projectTypeID)
                    .ToList();
                _originalInstrumentSpecificTemplates = instrumentSpecificTemplates;

                this.fpSpread2.ActiveSheet.RowCount = instrumentSpecificTemplates.Count;
                // Rows Setting
                for (int rowIndex = 0; rowIndex < instrumentSpecificTemplates.Count; rowIndex++)
                {
                    var instrument3DCataTypeColumn = instrumentSpecificTemplates[rowIndex];
                    this.fpSpread2.ActiveSheet.Cells[rowIndex, (int)menColumnColumn.Select].Value = false;
                    this.fpSpread2.ActiveSheet.Cells[rowIndex, (int)menColumnColumn.InstrumentSpecificTemplateID].Value = instrument3DCataTypeColumn.InstrumentSpecificTemplateID;
                    this.fpSpread2.ActiveSheet.Cells[rowIndex, (int)menColumnColumn.Name].Value = instrument3DCataTypeColumn.NameView;
                }
            }
        }

        private void MethodSettingRadGridView()
        {
            SpreadUtil.InitSpread(fpSpread1, marColumnWidth, marColumnName, mColumnLock, null, null, mColumnCellType, null, null, Color.LightGray, true, true);


            this.MethodBindingRadGridView();
        }

        private void MethodSettingRadGridViewAttributes()
        {
            SpreadUtil.InitSpread(fpSpread2, marColumnColumnWidth, marColumnColumnName, mColumnColumnLock, null, null, mColumnColumnCellType, null, null, Color.LightGray, true, true);
        }

        private void MethodDesignForm()
        {

            this.radDropDownListProjectType.ListElement.Font = new Font("Tahoma", (float)8.25, FontStyle.Regular);

        }
        //private void MethodTheme()
        //{
        //    //this.ThemeName = themeName;
        //    foreach (object control in this.Controls)
        //    {
        //        var radControl = control as RadControl;
        //        if (radControl != null)
        //        {
        //            radControl.ThemeName = SharedVariable.adminThemeName;
        //            radControl.Font = new Font("Tahoma", 8.25F);
        //            foreach (object subControl in radControl.Controls)
        //            {
        //                radControl = subControl as RadControl;
        //                if (radControl != null)
        //                {
        //                    radControl.ThemeName = SharedVariable.adminThemeName;
        //                    radControl.Font = new Font("Tahoma", 8.25F);
        //                    foreach (object subControl2 in radControl.Controls)
        //                    {
        //                        radControl = subControl2 as RadControl;
        //                        if (radControl != null)
        //                        {
        //                            radControl.ThemeName = SharedVariable.adminThemeName;
        //                            radControl.Font = new Font("Tahoma", 8.25F);

        //                        }
        //                    }
        //                }
        //            }
        //        }
        //    }
        //}


        private void MethodSaveBasicTemplate()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {                
                var instrumentBasicTemplates = new List<EDB_InstrumentBasicTemplate>();

                for (int i = 0; i < this.fpSpread1.ActiveSheet.RowCount; i++)
                {
                    var instrumentBasicTemplate = new EDB_InstrumentBasicTemplate();

                    instrumentBasicTemplate.InstrumentBasicTemplateID = Convert.ToInt32(this.fpSpread1.ActiveSheet.Cells[i, (int)menColumn.InstrumentBasicTemplateID].Value);
                    instrumentBasicTemplate.DeleteFlag = false;
                    instrumentBasicTemplate.Orders = (i + 1) * 100;
                    instrumentBasicTemplate.ModifiedDate = DateTime.Now;
                    instrumentBasicTemplate.Name = this.fpSpread1.ActiveSheet.Cells[i, (int)menColumn.Name].Text;
                    instrumentBasicTemplate.NameView = this.fpSpread1.ActiveSheet.Cells[i, (int)menColumn.Name].Text;

                    instrumentBasicTemplates.Add(instrumentBasicTemplate);
                }

                proxy.CreateInstrumentBasicTemplate(EDCS.WF.BSL.Variable.SharedVariable.IsIE, instrumentBasicTemplates);

                this.MethodBindingRadGridView();
            }
        }

        private void MethodSaveInstrumentSpecificTemplate()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                int projectType = Convert.ToInt32(this.radDropDownListProjectType.SelectedItem.Tag);

                var instrumentSpecificTemplates = new List<EDB_InstrumentSpecificTemplate>();
                for (int i = 0; i < this.fpSpread2.ActiveSheet.RowCount; i++)
                {
                    var instrumentSpecificTemplate = new EDB_InstrumentSpecificTemplate();

                    instrumentSpecificTemplate.InstrumentBasicTemplateID = 0;
                    instrumentSpecificTemplate.DeleteFlag = false;
                    instrumentSpecificTemplate.Orders = (i + 1) * 100;
                    instrumentSpecificTemplate.ModifiedDate = DateTime.Now;
                    instrumentSpecificTemplate.Name = this.fpSpread2.ActiveSheet.Cells[i, (int)menColumnColumn.Name].Text;
                    instrumentSpecificTemplate.NameView = this.fpSpread2.ActiveSheet.Cells[i, (int)menColumnColumn.Name].Text;
                    instrumentSpecificTemplate.InstrumentBasicTemplateID = Convert.ToInt32(this.fpSpread1.ActiveSheet.Cells[this.fpSpread1.ActiveSheet.ActiveRowIndex, (int)menColumn.InstrumentBasicTemplateID].Value);
                    instrumentSpecificTemplate.ProjectType = projectType;
                    instrumentSpecificTemplates.Add(instrumentSpecificTemplate);
                }

                proxy.CreateInstrumentSpecificTemplate(EDCS.WF.BSL.Variable.SharedVariable.IsIE, instrumentSpecificTemplates);

                this.MethodBindingRadGridViewAttributes();
            }
        }


        private void radDropDownListProjectType_SelectedIndexChanged(object sender, PositionChangedEventArgs e)
        {
            this.MethodBindingRadGridView();
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


        private void fpSpread1_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            //if (SaveChangesColumns())
            //{
            //    SaveData();
            //}

            MethodBindingRadGridViewAttributes();
        }

        private void radButtonTypeSave_Click(object sender, EventArgs e)
        {
            MethodSaveBasicTemplate();
        }

        private void radButtonColumnSave_Click(object sender, EventArgs e)
        {
            MethodSaveInstrumentSpecificTemplate();
        }

    }
}
