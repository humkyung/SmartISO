using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
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
    public partial class UserControlEDBShareRadGridView : UserControlBase
    {
        string themeName = "Windows8";

        int ListTypeInstrumentProcessData = 3;

        public UserControlEDBShareRadGridView()
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
            this.radDropDownListListType.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListListType_SelectedIndexChanged);
            this.radDropDownListProject.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListProject_SelectedIndexChanged);

            this.MethodDesignForm();
            this.MethodSettingRadGridView();
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

                int? basicTemplateID = null;
                int? specificTemplateID = null;

                switch (this._entityTagNode.EnumFormName)
                {
                    #region CaseDynamic

                    case BSL.Enums.EnumFormName.CaseDynamic:

                        if (this._entityEDBInformation.ListType.ListTypeID == (int)EnumNameCheck.ProcessEquipmentList && this.radGridView1.Columns["PackageFlag"] == null)
                        {
                            this.radGridView1.Columns.Add(new GridViewCheckBoxColumn()
                            {
                                Name = "PackageFlag",
                                HeaderText = "PackageFlag",
                                TextAlignment = ContentAlignment.MiddleCenter,
                                DataType = typeof(bool),
                                Width = 70,
                            });
                        }
                        else if (this._entityEDBInformation.ListType.ListTypeID != (int)EnumNameCheck.ProcessEquipmentList && this.radGridView1.Columns["PackageFlag"] != null)
                        {
                            this.radGridView1.Columns["PackageFlag"].IsVisible = false;
                        }
                        else if (this._entityEDBInformation.ListType.ListTypeID == (int)EnumNameCheck.ProcessEquipmentList && this.radGridView1.Columns["PackageFlag"] != null)
                        {
                            this.radGridView1.Columns["PackageFlag"].IsVisible = true;
                        }

                        if (_entityEDBInformation.ListType.ListTypeID.Equals(ListTypeInstrumentProcessData))
                        {
                            if (radDropDownListBasicTemplate.SelectedItem != null)
                                basicTemplateID = (int)radDropDownListBasicTemplate.SelectedItem.Tag;
                            if (radDropDownListSpecificTemplate.SelectedItem != null)
                                specificTemplateID = (int)radDropDownListSpecificTemplate.SelectedItem.Tag;
                        }
                        var caseDynamics = proxy.ReadCaseDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, Convert.ToInt32(this.radDropDownListProject.SelectedValue), Convert.ToInt32(this.radDropDownListListType.SelectedValue), null, basicTemplateID, specificTemplateID).ToList();

                        // Rows Setting
                        foreach (var caseDynamic in caseDynamics)
                        {
                            //this.radGridView1.Rows.Add(caseDynamic.CaseDynamicID, caseDynamic.Name, caseDynamic.NameView, caseDynamic.VisibleFlag, caseDynamic.EquipmentTypeDynamicID);
                            this.radGridView1.Rows.Add(false, caseDynamic.CaseDynamicID, caseDynamic.Name, caseDynamic.NameView, caseDynamic.VisibleFlag, caseDynamic.PackageFlag);
                        }
                        break;

                    #endregion
                    #region CaseDynamicDefault

                    case BSL.Enums.EnumFormName.CaseDynamicDefault:
                        if (this.radDropDownListListType.Text.Equals("Equipment List (Process)") && this.radGridView1.Columns["PackageFlag"] == null)
                        {
                            this.radGridView1.Columns.Add(new GridViewCheckBoxColumn()
                            {
                                Name = "PackageFlag",
                                HeaderText = "PackageFlag",
                                TextAlignment = ContentAlignment.MiddleCenter,
                                DataType = typeof(bool),
                                Width = 70,
                            });
                        }
                        else if (!this.radDropDownListListType.Text.Equals("Equipment List (Process)") && this.radGridView1.Columns["PackageFlag"] != null)
                        {
                            this.radGridView1.Columns["PackageFlag"].IsVisible = false;
                        }
                        else if (this.radDropDownListListType.Text.Equals("Equipment List (Process)") && this.radGridView1.Columns["PackageFlag"] != null)
                        {
                            this.radGridView1.Columns["PackageFlag"].IsVisible = true;
                        }
                        else if (this.radGridView1.Columns["PackageFlag"] != null)
                        {
                            this.radGridView1.Columns["PackageFlag"].IsVisible = false;
                        }

                        if (_entityEDBInformation.ListType.ListTypeID.Equals(ListTypeInstrumentProcessData))
                        {
                            if (radDropDownListBasicTemplate.SelectedItem != null)
                                basicTemplateID = (int)radDropDownListBasicTemplate.SelectedItem.Tag;
                            if (radDropDownListSpecificTemplate.SelectedItem != null)
                                specificTemplateID = (int)radDropDownListSpecificTemplate.SelectedItem.Tag;
                        }
                        var caseDynamicDefaults = proxy.ReadCaseDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE).Where(w => w.ListTypeID == Convert.ToInt32(this.radDropDownListListType.SelectedValue)
                            && (basicTemplateID == null ? true : w.InstrumentBasicTemplateID == basicTemplateID.Value)
                            && (specificTemplateID == null ? true : w.InstrumentSpecificTemplateID == specificTemplateID)).ToList();
                        foreach (var caseDynamicDefault in caseDynamicDefaults)
                        {
                            this.radGridView1.Rows.Add(false, caseDynamicDefault.CaseDynamicDefaultID, caseDynamicDefault.Name, caseDynamicDefault.NameView, caseDynamicDefault.VisibleFlag, caseDynamicDefault.PackageFlag);
                        }

                        break;
                    #endregion
                    #region CommonColumnDynamic

                    case BSL.Enums.EnumFormName.CommonColumnDynamic:

                        var commonColumnDynamics = proxy.ReadCommonColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, Convert.ToInt32(this.radDropDownListListType.SelectedValue))
                            //.Where(w => w.VisibleFlag.Equals(true))
                            .ToList();
                        // Rows Setting
                        foreach (var commonColumnDynamic in commonColumnDynamics)
                        {
                            this.radGridView1.Rows.Add(false, commonColumnDynamic.CommonColumnDynamicID, commonColumnDynamic.Name, commonColumnDynamic.NameView, commonColumnDynamic.CommonUnitID);
                        }

                        break;

                    #endregion
                    #region CommonSystemDynamic

                    case BSL.Enums.EnumFormName.CommonSystemDynamic:

                        // Columns Setting
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "CommonSystemDynamicID",
                            IsVisible = false,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "NameView",
                            HeaderText = "NameView",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            MaxLength = 50,
                        });
                        var commonSystemDynamics = proxy.ReadCommonSystemDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false)
                            .Where(w => w.VisibleFlag.Equals(true))
                            .ToList();
                        // Rows Setting
                        foreach (var commonSystemDynamic in commonSystemDynamics)
                        {
                            this.radGridView1.Rows.Add(commonSystemDynamic.CommonSystemDynamicID, commonSystemDynamic.NameView);
                        }

                        break;

                    #endregion
                    #region EquipmentTypeMapping

                    case BSL.Enums.EnumFormName.EquipmentTypeMappingMisc:
                    case BSL.Enums.EnumFormName.EquipmentTypeMappingPackage:
                        string caseDynamicName = "";
                        if (this._entityTagNode.EnumFormName == BSL.Enums.EnumFormName.EquipmentTypeMappingPackage) caseDynamicName = "pac";
                        else if (this._entityTagNode.EnumFormName == BSL.Enums.EnumFormName.EquipmentTypeMappingMisc) caseDynamicName = "mis";

                        var caseDynamicsEquipmentTypeMapping = proxy.ReadCaseDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, Convert.ToInt32(this.radDropDownListProject.SelectedValue), Convert.ToInt32(this.radDropDownListListType.SelectedValue));
                        var column = this.radGridView1.Columns[0] as GridViewComboBoxColumn;
                        var equipmentTypeDynamicAll = proxy.ReadEquipmentTypeDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, Convert.ToInt32(this.radDropDownListProject.SelectedValue), Convert.ToInt32(this.radDropDownListListType.SelectedValue));
                        var equipmentTypeDynamicDefaults = proxy.ReadEquipmentTypeDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.ListTypeID == Convert.ToInt32(this.radDropDownListListType.SelectedValue)).ToList();

                        var caseDynamicSingle = caseDynamicsEquipmentTypeMapping.SingleOrDefault(s => s.Name == caseDynamicName);
                        if (caseDynamicSingle != null)
                        {
                            column.DataSource = equipmentTypeDynamicAll.Where(w => w.ParentID == 0);
                            var ignoreEquipmentTypeDynamics = equipmentTypeDynamicDefaults.Where(w => w.CaseDynamicName == caseDynamicName).ToList();
                            var equipmentTypeDynamics = equipmentTypeDynamicAll.Where(w => w.CaseDynamicID == caseDynamicSingle.CaseDynamicID && w.ParentID == 0).ToList();
                            foreach (var listEquipmentTypeDynamic in equipmentTypeDynamics)
                            {
                                var locked = false;
                                var ignoreCount = ignoreEquipmentTypeDynamics.Where(w => w.CaseDynamicName == caseDynamicSingle.Name && w.Name.Length < 5 && w.Name.Substring(0, 2) == listEquipmentTypeDynamic.GroupCode && w.Name.Substring(2, 2) == listEquipmentTypeDynamic.Name).Count();
                                if (ignoreCount == 1)
                                {
                                    locked = true;
                                }
                                this.radGridView1.Rows.Add(listEquipmentTypeDynamic.EquipmentTypeDynamicID, locked);
                                this.radGridView1.Rows[this.radGridView1.Rows.Count - 1].Cells["EquipmentTypeDynamicID"].ReadOnly = locked;
                            }
                        }
                        break;

                    #endregion
                    #region ReplaceDynamic

                    case BSL.Enums.EnumFormName.ReplaceDynamic:
                        var replaceDynamics = proxy.ReadReplaceDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, Convert.ToInt32(this.radDropDownListProject.SelectedValue), Convert.ToInt32(this.radDropDownListListType.SelectedValue))
                            //.Where(w => w.VisibleFlag.Equals(true))
                            .ToList();
                        // Rows Setting
                        foreach (var replaceDynamic in replaceDynamics)
                        {
                            this.radGridView1.Rows.Add(replaceDynamic.ReplaceID, replaceDynamic.CommonColumnTypeID, replaceDynamic.Name, replaceDynamic.NameView, replaceDynamic.Value);
                        }

                        break;

                    #endregion
                    #region ReplaceDynamicDefault

                    case BSL.Enums.EnumFormName.ReplaceDynamicDefault:
                        var replaceDynamicDefaults = proxy.ReadReplaceDynamicDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, Convert.ToInt32(this.radDropDownListListType.SelectedValue))
                            //.Where(w => w.VisibleFlag.Equals(true))
                            .ToList();
                        // Rows Setting
                        foreach (var replaceDynamicdefault in replaceDynamicDefaults)
                        {
                            this.radGridView1.Rows.Add(replaceDynamicdefault.ReplaceDefaultID, replaceDynamicdefault.CommonColumnTypeID, replaceDynamicdefault.Name, replaceDynamicdefault.NameView, replaceDynamicdefault.Value);
                        }

                        break;

                    #endregion
                    case BSL.Enums.EnumFormName.EquipmentInquiryPackageNo:
                        List<EDB_EquipmentInquiryPackageNo> InquiryPkgNos = new List<EDB_EquipmentInquiryPackageNo>();
                        var Project = (EntityEDB_Project)this.radDropDownListProject.SelectedItem.DataBoundItem;

                        proxy.ReadPKGNoFromDMCS(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref InquiryPkgNos, Project.No, Project.Type == "Project" ? "2" : "1", disciplineName: "C");
                        proxy.ReadPKGNoFromDMCS(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref InquiryPkgNos, Project.No, Project.Type == "Project" ? "2" : "1", disciplineName: "Q");
                        proxy.ReadPKGNoFromDMCS(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref InquiryPkgNos, Project.No, Project.Type == "Project" ? "2" : "1", disciplineName: "M");

                        foreach (var inquirypkgno in InquiryPkgNos)
                        {
                            this.radGridView1.Rows.Add(inquirypkgno.EquipmentInquiryPackageNoID, inquirypkgno.JobNo, inquirypkgno.CostCode, inquirypkgno.Description);
                        }
                        break;
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

                this.radDropDownListListType.Enabled = true;
                this.radDropDownListProject.Enabled = true;


                switch (this._entityTagNode.EnumFormName)
                {
                    #region CaseDynamicDefault (Bulk Import -> Sheet)

                    case BSL.Enums.EnumFormName.CaseDynamicDefault:

                        this.MethodSettingDropDownListVisible(1);
                        this.MethodSettingDropDownListVisibleInstrument();
                        this.radGridView1.Size = new Size(698, 460);
                        this.radGridView1.MinimumSize = new Size(698, 460);
                        this.radGridView1.Location = new Point(0, 40);
                        this.radDropDownListListType.ValueMember = "ListTypeID";
                        this.radDropDownListListType.DisplayMember = "ComplexNameView";
                        this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                        this.radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;

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
                            Name = "CaseDynamicDefaultID",
                            IsVisible = false,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "Name",
                            HeaderText = "Key",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            MaxLength = 10,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "NameView",
                            HeaderText = "Sheet Name",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            MaxLength = 50,
                            //AllowSort = false
                            //Is
                        });
                        this.radGridView1.Columns.Add(new GridViewCheckBoxColumn()
                        {
                            Name = "Visible",
                            HeaderText = "View",
                            TextAlignment = ContentAlignment.MiddleCenter,
                            DataType = typeof(bool),
                            Width = 10,
                            //AllowSort = false,
                        });
                        this.radGridView1.Columns.Add(new GridViewCheckBoxColumn()
                        {
                            Name = "PackageFlag",
                            HeaderText = "PackageFlag",
                            TextAlignment = ContentAlignment.MiddleCenter,
                            DataType = typeof(bool),
                            Width = 30,
                            //AllowSort = false,
                        });

                        this.MethodBindingRadGridView();


                        this.radDropDownListListType.SelectedIndexChanged += (ss, ee) =>
                        {
                            this.radDropDownListListType_SelectedIndexChanged(ss, ee);
                        };

                        break;

                    #endregion
                    #region CaseDynamic (Engineering Data -> Common -> Sheet)

                    case BSL.Enums.EnumFormName.CaseDynamic:

                        this.MethodSettingDropDownListVisible(0);
                        this.MethodSettingDropDownListVisibleInstrument();
                        //this.radGridView1.Size = new Size(600, 500);
                        //this.radGridView1.MinimumSize = new Size(600, 500);
                        this.radGridView1.Location = new Point(0, 60);
                        this.radDropDownListProject.ValueMember = "ProjectID";
                        this.radDropDownListProject.DisplayMember = "ComplexNameView";
                        var projects = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true).ToList();
                        projects.ConvertAll(ca => ca.ComplexNameView = ca.ComplexNameView + " - " + ca.Type);
                        this.radDropDownListProject.DataSource = projects;
                        this.radDropDownListProject.SelectedValue = this._entityEDBInformation.Project.ProjectID;

                        this.radDropDownListListType.ValueMember = "ListTypeID";
                        this.radDropDownListListType.DisplayMember = "ComplexNameView";
                        this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                        this.radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;
                        //this.radPanelListType.Controls.Add(this.radButtonUp);
                        //this.radPanelListType.Controls.Add(this.radButtonDown);
                        //this.radButtonUp.Location = new System.Drawing.Point(300, 2);
                        //this.radButtonDown.Location = new System.Drawing.Point(330, 2);

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
                            Name = "CaseDynamicID",
                            IsVisible = false,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "Name",
                            HeaderText = "Key",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            MaxLength = 10,
                            IsVisible = true,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "NameView",
                            HeaderText = "Sheet Name",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            MaxLength = 50,
                            AllowSort = false
                            //Is
                        });
                        this.radGridView1.Columns.Add(new GridViewCheckBoxColumn()
                        {
                            Name = "Visible",
                            HeaderText = "View",
                            TextAlignment = ContentAlignment.MiddleCenter,
                            DataType = typeof(bool),
                            Width = 10,
                            AllowSort = false,
                            IsVisible = false,
                        });

                        this.radGridView1.Columns.Add(new GridViewCheckBoxColumn()
                        {
                            Name = "PackageFlag",
                            HeaderText = "PackageFlag",
                            TextAlignment = ContentAlignment.MiddleCenter,
                            DataType = typeof(bool),
                            Width = 10,

                            //AllowSort = false,
                        });


                        //this._entityEDBInformation.Project
                        //  this._entityEDBInformation.ListType
                        //this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                        //{
                        //    Name = "EquipmentTypeDynamicID",
                        //    HeaderText = "EquipmentType",
                        //    TextAlignment = ContentAlignment.MiddleCenter,
                        //    //DataSource = proxy.ReadEquipmentTypeDynamics(false, Convert.ToInt32(this.radDropDownList1.SelectedValue), Convert.ToInt32(this.radDropDownList2.SelectedValue)).Where(w => w.ParentID.Equals(0)).ToList(),
                        //    //DataType = typeof(bool),
                        //    Width = 20,
                        //    AllowSort = false,

                        //    ValueMember = "EquipmentTypeDynamicID",
                        //    DisplayMember = "NameView",
                        //    AutoCompleteMode = AutoCompleteMode.Append,
                        //    DropDownStyle = RadDropDownStyle.DropDown,

                        //    //IsVisible = this._entityFrmShareRadGridView.ListType.Name.Equals(Enum.GetName(typeof(EnumNameCheck), EnumNameCheck.ProcessEquipmentList)) ? true : false,
                        //    IsVisible = false,
                        //});

                        this.MethodBindingRadGridView();

                        this.radDropDownListListType.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListListType_SelectedIndexChanged);
                        this.radDropDownListProject.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListProject_SelectedIndexChanged);
                        this.radDropDownListListType.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListListType_SelectedIndexChanging);
                        this.radDropDownListProject.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListProject_SelectedIndexChanging);
                        break;

                    #endregion
                    
                    #region DMCS ExtractionXML

                    case BSL.Enums.EnumFormName.DMCSExtractionXML:
                        this.radDropDownListProject.ValueMember = "ProjectID";
                        this.radDropDownListProject.DisplayMember = "ComplexNameView";
                        this.radDropDownListProject.DataSource = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true).ToList();
                        this.radDropDownListProject.SelectedValue = this._entityEDBInformation.Project.CaseProjectID;

                        this.radDropDownListListType.ValueMember = "ListTypeID";
                        this.radDropDownListListType.DisplayMember = "ComplexNameView";
                        this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                        this.radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;

                        this.radPanelListType.Controls.Add(this.radButtonDMCSExtraction);

                        this.radButtonDMCSExtraction.Location = new Point(300, 2);
                        this.radButtonDMCSExtraction.Visible = true;

                        this.radDropDownListProject.SelectedIndexChanged += (ss, ee) =>
                        {
                            this.radDropDownListProject_SelectedIndexChanged(ss, ee);
                        };
                        this.radDropDownListListType.SelectedIndexChanged += (ss, ee) =>
                        {
                            this.radDropDownListListType_SelectedIndexChanged(ss, ee);
                        };
                        this.radDropDownListListType.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListListType_SelectedIndexChanging);
                        this.radDropDownListProject.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListProject_SelectedIndexChanging);
                        break;
                    #endregion

                    #region ReplaceDynamic

                    case BSL.Enums.EnumFormName.ReplaceDynamic:
                        this.radDropDownListProject.ValueMember = "ProjectID";
                        this.radDropDownListProject.DisplayMember = "ComplexNameView";
                        this.radDropDownListProject.DataSource = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true).ToList();
                        this.radDropDownListProject.SelectedValue = this._entityEDBInformation.Project.CaseProjectID;

                        this.radDropDownListListType.ValueMember = "ListTypeID";
                        this.radDropDownListListType.DisplayMember = "ComplexNameView";
                        this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                        this.radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;
                        // Columns Setting
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "ReplaceID",
                            IsVisible = false,
                        });
                        this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                        {
                            Name = "CommonColumnType",
                            HeaderText = "CommonColumnType",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            DataSource = proxy.ReadCommonColumnTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false),
                            ValueMember = "CommonColumnTypeID",
                            DisplayMember = "NameView",
                            AutoCompleteMode = AutoCompleteMode.Append,
                            DropDownStyle = RadDropDownStyle.DropDown,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "Name",
                            HeaderText = "Name",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            MaxLength = 25,
                        });

                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "NameView",
                            HeaderText = "NameView",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            MaxLength = 25,
                        });
                        this.radGridView1.Columns.Add(new GridViewDecimalColumn()
                        {
                            Name = "Value",
                            HeaderText = "Value",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            DataType = typeof(double),
                            Maximum = int.MaxValue,
                            Minimum = 0,
                            ShowUpDownButtons = true,
                            Step = 1,
                            DecimalPlaces = 5,
                            FormatString = "{0:F5}",
                        });
                        this.MethodBindingRadGridView();

                        this.radDropDownListProject.SelectedIndexChanged += (ss, ee) =>
                        {
                            this.radDropDownListProject_SelectedIndexChanged(ss, ee);
                        };
                        this.radDropDownListListType.SelectedIndexChanged += (ss, ee) =>
                        {
                            this.radDropDownListListType_SelectedIndexChanged(ss, ee);
                        };
                        this.radDropDownListListType.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListListType_SelectedIndexChanging);
                        this.radDropDownListProject.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListProject_SelectedIndexChanging);
                        break;

                    #endregion
                    #region ReplaceDynamicDefault

                    case BSL.Enums.EnumFormName.ReplaceDynamicDefault:

                        this.MethodSettingDropDownListVisible(1);
                        this.radDropDownListListType.ValueMember = "ListTypeID";
                        this.radDropDownListListType.DisplayMember = "ComplexNameView";
                        this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                        this.radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;

                        // Columns Setting
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "ReplacedynamicDefaultID",
                            IsVisible = false,
                        });
                        this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                        {
                            Name = "CommonColumnType",
                            HeaderText = "CommonColumnType",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            DataSource = proxy.ReadCommonColumnTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false),
                            ValueMember = "CommonColumnTypeID",
                            DisplayMember = "NameView",
                            AutoCompleteMode = AutoCompleteMode.Append,
                            DropDownStyle = RadDropDownStyle.DropDown,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "Name",
                            HeaderText = "Name",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            MaxLength = 25,
                        });

                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "NameView",
                            HeaderText = "NameView",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            MaxLength = 25,
                        });
                        this.radGridView1.Columns.Add(new GridViewDecimalColumn()
                        {
                            Name = "Value",
                            HeaderText = "Value",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            DataType = typeof(double),
                            Maximum = int.MaxValue,
                            Minimum = 0,
                            ShowUpDownButtons = true,
                            Step = 1,
                            DecimalPlaces = 5,
                            FormatString = "{0:F5}",
                        });
                        this.MethodBindingRadGridView();

                        this.radDropDownListListType.SelectedIndexChanged += (ss, ee) =>
                        {
                            this.radDropDownListListType_SelectedIndexChanged(ss, ee);
                        };
                        this.radDropDownListListType.SelectedIndexChanging += new Telerik.WinControls.UI.Data.PositionChangingEventHandler(radDropDownListListType_SelectedIndexChanging);
                        break;

                    #endregion

                    #region EquipmentTypeDynamic

                    //case EnumFormName.EquipmentTypeDynamic:

                    //    this.radDropDownList1.ValueMember = "ProjectID";
                    //    this.radDropDownList1.DisplayMember = "ComplexNameView";
                    //    this.radDropDownList1.DataSource = proxy.ReadProjects(false).Where(w => w.CaseMainFlag == true).ToList();
                    //    this.radDropDownList1.SelectedValue = this._entityFrmShareRadGridView.Project.CaseProjectID;

                    //    this.radDropDownList2.ValueMember = "ListTypeID";
                    //    this.radDropDownList2.DisplayMember = "NameView";
                    //    this.radDropDownList2.DataSource = proxy.ReadListTypes(false);
                    //    this.radDropDownList2.SelectedValue = this._entityFrmShareRadGridView.ListType.ListTypeID;

                    //    // Columns Setting
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "EquipmentTypeDynamicID",
                    //        IsVisible = false,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "Name",
                    //        HeaderText = "Key",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        //MaxLength = 3,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "Eq. Type Name",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });
                    //    //this.radGridView1.Columns.Add(new Telerik.WinControls.UI.GridViewCheckBoxColumn()
                    //    //{
                    //    //    Name = "Visible",
                    //    //    HeaderText = "View",
                    //    //    TextAlignment = ContentAlignment.MiddleCenter,
                    //    //    DataType = typeof(bool),
                    //    //    Width = 10
                    //    //});
                    //    this.MethodBindingRadGridView();

                    //    this.radDropDownList1.SelectedIndexChanging += (ss, ee) =>
                    //    {
                    //        this.radDropDownList1_SelectedIndexChanging(ss, ee);
                    //    };
                    //    this.radDropDownList1.SelectedIndexChanged += (ss, ee) =>
                    //    {
                    //        this.radDropDownList1_SelectedIndexChanged(ss, ee);
                    //    };
                    //    this.radDropDownList2.SelectedIndexChanging += (ss, ee) =>
                    //    {
                    //        this.radDropDownList2_SelectedIndexChanging(ss, ee);
                    //    };
                    //    this.radDropDownList2.SelectedIndexChanged += (ss, ee) =>
                    //    {
                    //        this.radDropDownList2_SelectedIndexChanged(ss, ee);
                    //    };

                    //    break;


                    #endregion
                    #region PowerEquipmentFileDynamic
                    //case EnumFormName.PowerEquipmentFileDynamic:

                    //    this.radLabel1.Visible = true;
                    //    this.radLabel2.Visible = true;
                    //    this.radLabel3.Visible = true;
                    //    this.radLabel4.Visible = true;
                    //    this.radLabel1.Text = this._entityFrmShareRadGridView.Project.ComplexNameView;
                    //    this.radLabel2.Text = this._entityFrmShareRadGridView.ListType.NameView;
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "PowerEquipmentFileID",
                    //        HeaderText = "PowerEquipmentFileID",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        IsVisible = false
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewBrowseColumn()
                    //    {
                    //        Name = "FileName",
                    //        HeaderText = "FileName",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        IsVisible = true,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewBrowseColumn()
                    //    {
                    //        Name = "FilePath",
                    //        HeaderText = "FilePath",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        IsVisible = false,
                    //    });

                    //    var powerEquipmentFileDynamics = proxy.ReadPowerEquipmentFileDynamics(false, this._entityFrmShareRadGridView.Project.CaseProjectID);
                    //    // Rows Setting
                    //    foreach (var powerEquipmentFileDynamics1 in powerEquipmentFileDynamics)
                    //    {
                    //        this.radGridView1.Rows.Add(
                    //              powerEquipmentFileDynamics1.PowerEquipmentFileDynamicID,
                    //              powerEquipmentFileDynamics1.FileName,
                    //              powerEquipmentFileDynamics1.FilePath
                    //            );
                    //        for (int i = 0; i < this.radGridView1.Columns.Count; i++)
                    //        {
                    //            this.radGridView1.Rows[this.radGridView1.Rows.Count - 1].Cells[i].ReadOnly = true;
                    //        }
                    //    }
                    //    break;
                    #endregion

                    #region ColumnDynamicDropDownList

                    //case EnumFormName.ColumnDynamicDropDownList:

                    //    this.radLabel1.Text = this._entityFrmShareRadGridView.Project.ComplexNameView;
                    //    this.radLabel2.Text = this._entityFrmShareRadGridView.ListType.NameView;
                    //    this.radDropDownList1.ValueMember = "ProjectID";
                    //    this.radDropDownList1.DisplayMember = "ComplexNameView";
                    //    this.radDropDownList1.DataSource = proxy.ReadProjects(false).Where(w => w.CaseMainFlag == true).ToList();
                    //    this.radDropDownList1.SelectedValue = this._entityFrmShareRadGridView.Project.CaseProjectID;

                    //    this.radDropDownList2.ValueMember = "ListTypeID";
                    //    this.radDropDownList2.DisplayMember = "NameView";
                    //    this.radDropDownList2.DataSource = proxy.ReadListTypes(false);
                    //    this.radDropDownList2.SelectedValue = this._entityFrmShareRadGridView.ListType.ListTypeID;

                    //    // Columns Setting

                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "ColumnDynamicDropDownListID",
                    //        IsVisible = false,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "Name",
                    //        HeaderText = "Name",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        IsVisible = false,

                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "NameView",
                    //        Width = 20,
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //    });

                    //    this.MethodBindingRadGridView();

                    //    this.radDropDownList1.SelectedIndexChanging += (ss, ee) =>
                    //    {
                    //        this.radDropDownList1_SelectedIndexChanging(ss, ee);
                    //    };
                    //    this.radDropDownList1.SelectedIndexChanged += (ss, ee) =>
                    //    {
                    //        this.radDropDownList1_SelectedIndexChanged(ss, ee);
                    //    };
                    //    this.radDropDownList2.SelectedIndexChanging += (ss, ee) =>
                    //    {
                    //        this.radDropDownList2_SelectedIndexChanging(ss, ee);
                    //    };
                    //    this.radDropDownList2.SelectedIndexChanged += (ss, ee) =>
                    //    {
                    //        this.radDropDownList2_SelectedIndexChanged(ss, ee);
                    //    };

                    //    break;


                    #endregion

                    #region MRPackageNo

                    //case EnumFormName.MRPkgNo:
                    //    this.radDropDownList1.Visible = false;
                    //    this.radDropDownList2.Visible = false;
                    //    this.radLabel1.Visible = true;
                    //    this.radLabel2.Visible = true;
                    //    this.radLabel3.Visible = true;
                    //    this.radLabel4.Visible = true;
                    //    this.radLabel1.Text = this._entityFrmShareRadGridView.Project.ComplexNameView;
                    //    this.radLabel2.Text = this._entityFrmShareRadGridView.ListType.NameView;

                    //    this.radDropDownList1.ValueMember = "ProjectID";
                    //    this.radDropDownList1.DisplayMember = "ComplexNameView";
                    //    this.radDropDownList1.DataSource = proxy.ReadProjects(false).Where(w => w.CaseMainFlag == true).ToList();
                    //    this.radDropDownList1.SelectedValue = this._entityFrmShareRadGridView.Project.CaseProjectID;

                    //    this.radDropDownList2.ValueMember = "ListTypeID";
                    //    this.radDropDownList2.DisplayMember = "NameView";
                    //    this.radDropDownList2.DataSource = proxy.ReadListTypes(false);
                    //    this.radDropDownList2.SelectedValue = this._entityFrmShareRadGridView.ListType.ListTypeID;

                    //    // Columns Setting
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "EqipmentInquiryPackageNoID",
                    //        IsVisible = false,
                    //    });

                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "SeqNo",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleLeft,
                    //        MaxLength = 3
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "Description",
                    //        HeaderText = "Description",
                    //        HeaderTextAlignment = ContentAlignment.MiddleLeft,
                    //        TextAlignment = ContentAlignment.MiddleLeft,
                    //    });


                    //    this.MethodBindingRadGridView();

                    //    this.radGridView1.Columns["SeqNo"].Sort(RadSortOrder.Ascending, false);

                    //    this.radDropDownList1.SelectedIndexChanging += (ss, ee) =>
                    //    {
                    //        this.radDropDownList1_SelectedIndexChanging(ss, ee);
                    //    };
                    //    this.radDropDownList1.SelectedIndexChanged += (ss, ee) =>
                    //    {
                    //        this.radDropDownList1_SelectedIndexChanged(ss, ee);
                    //    };
                    //    this.radDropDownList2.SelectedIndexChanging += (ss, ee) =>
                    //    {
                    //        this.radDropDownList2_SelectedIndexChanging(ss, ee);
                    //    };
                    //    this.radDropDownList2.SelectedIndexChanged += (ss, ee) =>
                    //    {
                    //        this.radDropDownList2_SelectedIndexChanged(ss, ee);
                    //    };

                    //    break;


                    #endregion

                    #region EquipmentInquiryPackageNo

                    case BSL.Enums.EnumFormName.EquipmentInquiryPackageNo:
                        this.radDropDownListProject.ValueMember = "ProjectID";
                        this.radDropDownListProject.DisplayMember = "ComplexNameView";
                        this.radDropDownListProject.DataSource = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true).ToList();
                        this.radDropDownListProject.SelectedValue = this._entityEDBInformation.Project.CaseProjectID;

                        this.radDropDownListListType.ValueMember = "ListTypeID";
                        this.radDropDownListListType.DisplayMember = "ComplexNameView";
                        this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                        this.radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;

                        this.radDropDownListListType.SelectedValue = 4;
                        this.radDropDownListListType.Enabled = false;

                        // Columns Setting
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "EqipmentInquiryPackageNoID",
                            IsVisible = false,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "JobNo",
                            HeaderText = "JobNo",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleLeft,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "CostCode",
                            HeaderText = "CostCode",
                            Width = 20,
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleLeft,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "SeqNo",
                            HeaderText = "SeqNo",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleLeft,
                            MaxLength = 3
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "Description",
                            HeaderText = "Description",
                            HeaderTextAlignment = ContentAlignment.MiddleLeft,
                            TextAlignment = ContentAlignment.MiddleLeft,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "MrNo",
                            HeaderText = "MrNo",
                            IsVisible = false,
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                        });

                        this.MethodBindingRadGridView();

                        //this.radGridView1.Columns["SeqNo"].Sort(RadSortOrder.Ascending, false);

                        this.radDropDownListProject.SelectedIndexChanging += (ss, ee) =>
                        {
                            this.radDropDownListProject_SelectedIndexChanging(ss, ee);
                        };
                        this.radDropDownListProject.SelectedIndexChanged += (ss, ee) =>
                        {
                            this.radDropDownListProject_SelectedIndexChanged(ss, ee);
                        };
                        break;


                    #endregion

                    #region ExcelReportLostSheetMapping

                    //case EnumFormName.ExcelReportLostSheetMapping:
                    //    this.radDropDownList1.Visible = false;
                    //    this.radDropDownList2.Visible = false;

                    //    this.radLabel1.Text = this._entityFrmShareRadGridView.Project.ComplexNameView;
                    //    this.radLabel2.Text = this._entityFrmShareRadGridView.ListType.NameView;

                    //    this.radDropDownList1.ValueMember = "ProjectID";
                    //    this.radDropDownList1.DisplayMember = "ComplexNameView";
                    //    this.radDropDownList1.DataSource = proxy.ReadProjects(false).Where(w => w.CaseMainFlag == true).ToList();
                    //    this.radDropDownList1.SelectedValue = this._entityFrmShareRadGridView.Project.CaseProjectID;

                    //    this.radDropDownList2.ValueMember = "ListTypeID";
                    //    this.radDropDownList2.DisplayMember = "NameView";
                    //    this.radDropDownList2.DataSource = proxy.ReadListTypes(false);
                    //    this.radDropDownList2.SelectedValue = this._entityFrmShareRadGridView.ListType.ListTypeID;

                    //    // Columns Setting
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "LostSheetName",
                    //        HeaderText = "ExcelSheetName",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleLeft,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                    //    {
                    //        Name = "ExcelSheetName",
                    //        HeaderText = "ExcelSheetName",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleLeft,
                    //        DataSource = this._entityFrmShareRadGridView.ExcelReportLostMappingSheetNames,
                    //    });
                    //    this.MethodBindingRadGridView();
                    //    break;
                    //#endregion

                    //#region PowerEquipmentService

                    //case EnumFormName.PowerEquipmentService:

                    //    this.radLabel1.Text = this._entityFrmShareRadGridView.Project.ComplexNameView;
                    //    this.radLabel2.Text = this._entityFrmShareRadGridView.ListType.NameView;
                    //    this.radDropDownList1.ValueMember = "ProjectID";
                    //    this.radDropDownList1.DisplayMember = "ComplexNameView";
                    //    this.radDropDownList1.DataSource = proxy.ReadProjects(false).Where(w => w.CaseMainFlag == true).ToList();
                    //    this.radDropDownList1.SelectedValue = this._entityFrmShareRadGridView.Project.CaseProjectID;

                    //    this.radDropDownList2.ValueMember = "ListTypeID";
                    //    this.radDropDownList2.DisplayMember = "NameView";
                    //    this.radDropDownList2.DataSource = proxy.ReadListTypes(false);
                    //    this.radDropDownList2.SelectedValue = this._entityFrmShareRadGridView.ListType.ListTypeID;

                    //    // Columns Setting

                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "PowerEquipmentServiceID",
                    //        IsVisible = false,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "Name",
                    //        HeaderText = "Name",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        IsVisible = false,

                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "Service",
                    //        Width = 20,
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //    });

                    //    this.MethodBindingRadGridView();

                    //    this.radDropDownList1.SelectedIndexChanging += (ss, ee) =>
                    //    {
                    //        this.radDropDownList1_SelectedIndexChanging(ss, ee);
                    //    };
                    //    this.radDropDownList1.SelectedIndexChanged += (ss, ee) =>
                    //    {
                    //        this.radDropDownList1_SelectedIndexChanged(ss, ee);
                    //    };
                    //    this.radDropDownList2.SelectedIndexChanging += (ss, ee) =>
                    //    {
                    //        this.radDropDownList2_SelectedIndexChanging(ss, ee);
                    //    };
                    //    this.radDropDownList2.SelectedIndexChanged += (ss, ee) =>
                    //    {
                    //        this.radDropDownList2_SelectedIndexChanged(ss, ee);
                    //    };

                    //    break;


                    #endregion
                    #region PowerEquipmentNational

                    //case EnumFormName.PowerEquipmentNational:

                    //    // Columns Setting
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "PowerEquipmentNationalID",
                    //        IsVisible = false,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "NameView",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });

                    //    var equipmentNationals = proxy.ReadPowerEquipmentNationals(false).ToList();
                    //    // Rows Setting
                    //    foreach (var equipmentNational in equipmentNationals)
                    //    {
                    //        this.radGridView1.Rows.Add(equipmentNational.PowerEquipmentNationalID, equipmentNational.NameView);
                    //    }

                    //    break;

                    #endregion
                    #region PowerEquipmentProjectType

                    //case EnumFormName.PowerEquipmentProjectType:

                    //    // Columns Setting
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "PowerEquipmentProjectTypeID",
                    //        IsVisible = false,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "NameView",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });

                    //    var powerEquipmentProjectTypes = proxy.ReadPowerEquipmentProjectTypes(false).ToList();
                    //    // Rows Setting
                    //    foreach (var powerEquipmentProjectType in powerEquipmentProjectTypes)
                    //    {
                    //        this.radGridView1.Rows.Add(powerEquipmentProjectType.PowerEquipmentProjectTypeID, powerEquipmentProjectType.NameView);
                    //    }

                    //    break;

                    #endregion

                    #region EquipmentTypeMapping

                    case BSL.Enums.EnumFormName.EquipmentTypeMappingPackage:
                    case BSL.Enums.EnumFormName.EquipmentTypeMappingMisc:
                        string caseDynamicName = "";
                        if (this._entityTagNode.EnumFormName == BSL.Enums.EnumFormName.EquipmentTypeMappingPackage) caseDynamicName = "pac";
                        else if (this._entityTagNode.EnumFormName == BSL.Enums.EnumFormName.EquipmentTypeMappingMisc) caseDynamicName = "mis";

                        this.MethodSettingDropDownListVisible(0);


                        this.radDropDownListProject.ValueMember = "ProjectID";
                        this.radDropDownListProject.DisplayMember = "ComplexNameView";
                        this.radDropDownListProject.DataSource = proxy.ReadProjects(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.CaseMainFlag == true)
                            .Where(w => w.ListTypeFlag.Contains(4)).ToList();
                        this.radDropDownListProject.SelectedValue = this._entityEDBInformation.Project.ProjectID;

                        this.radDropDownListListType.ValueMember = "ListTypeID";
                        this.radDropDownListListType.DisplayMember = "ComplexNameView";
                        this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                        //공정만 
                        this.radDropDownListListType.SelectedValue = 4;

                        this.radDropDownListListType.Enabled = false;


                        this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                        {
                            Name = "EquipmentTypeDynamicID",
                            HeaderText = "EquipmentType",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,

                            //DataSource = proxy.ReadCaseDynamics(false, this._entityFrmShareRadGridView.Project.ProjectID, this._entityFrmShareRadGridView.ListType.ListTypeID)
                            //.Where(w => w.Name.Substring(0, 1) == "s"),
                            ValueMember = "EquipmentTypeDynamicID",
                            DisplayMember = "NameView",
                            AutoCompleteMode = AutoCompleteMode.Append,
                            DropDownStyle = RadDropDownStyle.DropDown,

                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "Locked",
                            IsVisible = false,
                        });
                        this.MethodBindingRadGridView();


                        this.radDropDownListProject.SelectedIndexChanged += (ss, ee) =>
                        {
                            this.radDropDownListProject_SelectedIndexChanged(ss, ee);
                        };
                        break;

                    #endregion

                    #region CommonMultiProperty

                    //case EnumFormName.CommonMultiProperty:

                    //    // Columns Setting
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "CommonMultiPropertyID",
                    //        IsVisible = false,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "Name",
                    //        HeaderText = "Name",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "NameView",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });

                    //    var commonMultiProperties = proxy.ReadCommonMultiProperties(false)
                    //        //.Where(w => w.VisibleFlag.Equals(true))
                    //        .ToList();
                    //    // Rows Setting
                    //    foreach (var commonMultiProperty in commonMultiProperties)
                    //    {
                    //        this.radGridView1.Rows.Add(commonMultiProperty.CommonMultiPropertyID, commonMultiProperty.Name, commonMultiProperty.NameView);
                    //    }

                    //    break;

                    #endregion
                    #region CommonUnit

                    //case EnumFormName.CommonUnit:

                    //    // Columns Setting
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "CommonUnitID",
                    //        IsVisible = false,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "NameView",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });
                    //    //this.radGridView1.Columns.Add(new Telerik.WinControls.UI.GridViewDecimalColumn()
                    //    //{
                    //    //    Name = "Value",
                    //    //    HeaderText = "Value",
                    //    //    HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //    //    TextAlignment = ContentAlignment.MiddleCenter,
                    //    //    //
                    //    //    DataType = typeof(double),
                    //    //    Maximum = int.MaxValue,
                    //    //    Minimum = 0,
                    //    //    ShowUpDownButtons = true,
                    //    //    Step = 1,
                    //    //    DecimalPlaces = 5,
                    //    //    FormatString = "{0:F5}",
                    //    //});
                    //    this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                    //    {
                    //        Name = "Value",
                    //        HeaderText = "Value",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        DataSource = proxy.ReadCommonColumnTypes(false),
                    //        //DataType = typeof(bool),
                    //        Width = 20,
                    //        AllowSort = false,

                    //        ValueMember = "CommonColumnTypeID",
                    //        DisplayMember = "NameView",
                    //        AutoCompleteMode = AutoCompleteMode.Append,
                    //        DropDownStyle = RadDropDownStyle.DropDown,
                    //    });
                    //    var commonUnits = proxy.ReadCommonUnits(false)
                    //        .Where(w => w.VisibleFlag.Equals(true))
                    //        .ToList();
                    //    // Rows Setting
                    //    foreach (var commonUnit in commonUnits)
                    //    {
                    //        this.radGridView1.Rows.Add(commonUnit.CommonUnitID, commonUnit.NameView, commonUnit.CommonColumnTypeID);
                    //    }

                    //    break;

                    #endregion
                    #region CommonColumnDynamic

                    case BSL.Enums.EnumFormName.CommonColumnDynamic:

                        this.MethodSettingDropDownListVisible(1);

                        this.radDropDownListListType.ValueMember = "ListTypeID";
                        this.radDropDownListListType.DisplayMember = "ComplexNameView";
                        this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                        this.radDropDownListListType.SelectedValue = this._entityEDBInformation.ListType.ListTypeID;

                        var CommonUnits = proxy.ReadCommonUnits(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                        CommonUnits = CommonUnits.OrderBy(ob => ob.CommonUnitID).ToList();
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
                            Name = "CommonColumnDynamicID",
                            IsVisible = false,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "Name",
                            HeaderText = "Name",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            MaxLength = 25,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "NameView",
                            HeaderText = "NameView",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            MaxLength = 25,
                        });
                        this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                        {   
                            Name = "CommonUnitID",
                            HeaderText = "Unit",
                            DataSource = CommonUnits,
                            ValueMember = "CommonUnitID",
                            //DisplayMember = "NameView",
                            DisplayMember = "ComplexNameView",
                            //DataSource = proxy.ReadCommonUnits(false),
                            //
                        });

                        this.MethodBindingRadGridView();


                        this.radDropDownListListType.SelectedIndexChanged += (ss, ee) =>
                        {
                            this.radDropDownListListType_SelectedIndexChanged(ss, ee);
                        };

                        break;

                    #endregion
                    #region CommonDistrictType

                    //case EnumFormName.CommonDistrictType:

                    //    // Columns Setting
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "CommonDistrictTypeID",
                    //        IsVisible = false,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "Name",
                    //        HeaderText = "Name",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "NameView",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });

                    //    var commonDistrictTypes = proxy.ReadCommonDistrictTypes(false, this._entityFrmShareRadGridView.ListType.ListTypeID)
                    //        //.Where(w => w.VisibleFlag.Equals(true))
                    //        .ToList();
                    //    // Rows Setting
                    //    foreach (var commonDistrictType in commonDistrictTypes)
                    //    {
                    //        this.radGridView1.Rows.Add(commonDistrictType.CommonDistrictTypeID, commonDistrictType.Name, commonDistrictType.NameView);
                    //    }

                    //    break;

                    #endregion
                    #region CommonColumnType

                    //case EnumFormName.CommonColumnType:

                    //    // Columns Setting
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "CommonColumnTypeID",
                    //        IsVisible = false,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "Name",
                    //        HeaderText = "Name",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "NameView",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });

                    //    var commonColumnTypes = proxy.ReadCommonColumnTypes(false)
                    //        //.Where(w => w.VisibleFlag.Equals(true))
                    //        .ToList();
                    //    // Rows Setting
                    //    foreach (var commonColumnType in commonColumnTypes)
                    //    {
                    //        this.radGridView1.Rows.Add(commonColumnType.CommonColumnTypeID, commonColumnType.Name, commonColumnType.NameView);
                    //    }

                    //    break;

                    #endregion
                    #region CommonSystemDynamic

                    case BSL.Enums.EnumFormName.CommonSystemDynamic:

                        this.MethodSettingDropDownListVisible(1);

                        this.radDropDownListListType.ValueMember = "ListTypeID";
                        this.radDropDownListListType.DisplayMember = "ComplexNameView";
                        this.radDropDownListListType.DataSource = proxy.ReadListTypes(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false);
                        //발전만 
                        this.radDropDownListListType.SelectedValue = 5;

                        this.radDropDownListListType.Enabled = false;

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
                        // Columns Setting
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "CommonSystemDynamicID",
                            IsVisible = false,
                        });
                        this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                        {
                            Name = "NameView",
                            HeaderText = "NameView",
                            HeaderTextAlignment = ContentAlignment.MiddleCenter,
                            TextAlignment = ContentAlignment.MiddleCenter,
                            //
                            MaxLength = 50,
                        });
                        var commonSystemDynamics = proxy.ReadCommonSystemDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false)
                            .Where(w => w.VisibleFlag.Equals(true))
                            .ToList();
                        // Rows Settinga
                        int rowCount = 0;
                        foreach (var commonSystemDynamic in commonSystemDynamics)
                        {
                            this.radGridView1.Rows.AddNew();
                            this.radGridView1.Rows[rowCount].Cells["DeleteCheck"].Value = true;
                            this.radGridView1.Rows[rowCount].Cells["CommonSystemDynamicID"].Value = commonSystemDynamic.CommonSystemDynamicID;
                            this.radGridView1.Rows[rowCount].Cells["NameView"].Value = commonSystemDynamic.NameView;
                            rowCount++;
                            //this.radGridView1.Rows.Add(commonSystemDynamic.CommonSystemDynamicID, commonSystemDynamic.NameView);
                        }

                        break;

                    #endregion
                    #region Project

                    //case EnumFormName.Project:


                    //    var remarkTable = new DataTable("Table1");
                    //    remarkTable.Columns.Add("Value", typeof(int));
                    //    remarkTable.Columns.Add("Text", typeof(string));
                    //    foreach (EnumProjectRemark type in Enum.GetValues(typeof(EnumProjectRemark)))
                    //    {
                    //        remarkTable.Rows.Add((int)type, type.ToString());
                    //    }

                    //    var typeTable = new DataTable("Table1");
                    //    typeTable.Columns.Add("Value", typeof(int));
                    //    typeTable.Columns.Add("Text", typeof(string));
                    //    foreach (EnumProjectType2 type in Enum.GetValues(typeof(EnumProjectType2)))
                    //    {
                    //        typeTable.Rows.Add((int)type, type.ToString());
                    //    }

                    //    // Columns Setting
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "ProjectID",
                    //        IsVisible = false,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "ProjectNo",
                    //        HeaderText = "ProjectNo",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 12,
                    //    });

                    //    this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                    //    {
                    //        Name = "Type",
                    //        HeaderText = "Type",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        DataSource = new string[] { "Project", "Proposal" },
                    //        //MaxLength = 6,
                    //    });

                    //    this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                    //    {
                    //        Name = "Type2",
                    //        HeaderText = "Type2",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        DataSource = typeTable,
                    //        ValueMember = "Value",
                    //        DisplayMember = "Text",
                    //        DropDownStyle = RadDropDownStyle.DropDownList,
                    //        //MaxLength = 6,
                    //    });

                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "Name",
                    //        HeaderText = "Key",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 50,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "Project Name",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 50,
                    //    });

                    //    this.radGridView1.Columns.Add(new GridViewDateTimeColumn()
                    //    {
                    //        Name = "CreationDate",
                    //        HeaderText = "Creation Date",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,

                    //        FormatString = "{0:d}",
                    //        Format = DateTimePickerFormat.Short,
                    //        FormatInfo = new CultureInfo("ko-KR"),
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewDateTimeColumn()
                    //    {
                    //        Name = "ClosingDate",
                    //        HeaderText = "Closing Date",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,

                    //        FormatString = "{0:d}",
                    //        Format = DateTimePickerFormat.Short,
                    //        FormatInfo = new CultureInfo("ko-KR"),
                    //    });

                    //    this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                    //    {
                    //        Name = "Remark",
                    //        HeaderText = "Remark",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        DataSource = remarkTable,
                    //        ValueMember = "Value",
                    //        DisplayMember = "Text",
                    //        DropDownStyle = RadDropDownStyle.DropDownList,

                    //        IsVisible = this._entityFrmShareRadGridView.Employee.Authority == Convert.ToByte(EnumAuthorityType.SuperAdmin)
                    //         ? true : false,
                    //        //MaxLength = 6,
                    //    });

                    //    var projects = proxy.ReadProjects(false).Where(w => w.CaseMainFlag == true)
                    //        .OrderBy(ob => ob.TestFlag).ThenBy(tb => tb.CompleteFlag).ThenByDescending(t => t.No)
                    //        .ToList();
                    //    // Rows Setting


                    //    foreach (var project in projects)
                    //    {
                    //        EnumProjectRemark remark;
                    //        if (project.TestFlag) remark = EnumProjectRemark.Test;
                    //        else if (project.CompleteFlag) remark = EnumProjectRemark.Completed;
                    //        else remark = EnumProjectRemark.OnGoing;

                    //        this.radGridView1.Rows.Add(project.ProjectID, project.No, project.Type, project.Type2, project.Name, project.NameView, project.CreationDate, project.ClosingDate, (int)remark);
                    //    }
                    //    break;

                    #endregion
                    #region ProjectSummary

                    //case EnumFormName.ProjectSummary:

                    //    // Columns Setting
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "ProjectID",
                    //        IsVisible = false,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "Project Name",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleLeft,
                    //        ReadOnly = true,
                    //        //
                    //        MaxLength = 25,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                    //    {
                    //        Name = "PowerEquipmentProjectType",
                    //        HeaderText = "Type",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,

                    //        DataSource = proxy.ReadPowerEquipmentProjectTypes(false).Select(s => s.NameView),
                    //        //ValueMember = "PowerEquipmentProjectTypeID",
                    //        //DisplayMember = "NameView",
                    //        DropDownStyle = RadDropDownStyle.DropDown,
                    //        //
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "Capacity",
                    //        HeaderText = "Capacity (MW)",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                    //    {
                    //        Name = "PowerEquipmentNational",
                    //        HeaderText = "Country",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,

                    //        DataSource = proxy.ReadPowerEquipmentNationals(false),
                    //        //ValueMember = "PowerEquipmentNationalID",
                    //        DisplayMember = "NameView",
                    //        DropDownStyle = RadDropDownStyle.DropDownList,
                    //        //
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "PrimaryFuelType",
                    //        HeaderText = "PrimaryFuelType",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "CoolingWaterSource",
                    //        HeaderText = "CoolingWaterSource",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });
                    //    var projects1 = proxy.ReadProjects(false).Where(w => w.CaseMainFlag == true)
                    //        //.Where(w => w.VisibleFlag.Equals(true))
                    //        .ToList();
                    //    // Rows Setting
                    //    foreach (var project in projects1)
                    //    {
                    //        this.radGridView1.Rows.Add(
                    //            project.ProjectID, project.ComplexNameView
                    //            , project.PowerEquipmentProjectType, project.Capacity, project.PowerEquipmentNational, project.PrimaryFuelType, project.CoolingWaterSource);
                    //    }

                    //    break;

                    #endregion
                    #region ListType

                    //case EnumFormName.ListType:

                    //    this.radGridView1.AllowAddNewRow = false;
                    //    this.radGridView1.AllowDeleteRow = false;

                    //    // Columns Setting
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "ListTypeID",
                    //        IsVisible = false,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "Name",
                    //        HeaderText = "Key",
                    //        //IsVisible = false,
                    //        ReadOnly = true,
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "List Type",
                    //        ReadOnly = true,
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        MaxLength = 25,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewComboBoxColumn()
                    //    {
                    //        Name = "DisciplineID",
                    //        HeaderText = "Department",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,

                    //        DataSource = proxy.ReadDisciplines(false),
                    //        ValueMember = "DisciplineID",
                    //        DisplayMember = "NameView",
                    //        AutoCompleteMode = AutoCompleteMode.SuggestAppend,
                    //        DropDownStyle = RadDropDownStyle.DropDown,
                    //    });

                    //    var listTypes = proxy.ReadListTypes(false)
                    //        //.Where(w => w.VisibleFlag.Equals(true))
                    //        .ToList();
                    //    // Rows Setting
                    //    foreach (var listType in listTypes)
                    //    {
                    //        this.radGridView1.Rows.Add(listType.ListTypeID, listType.Name, listType.NameView, listType.DisciplineID);
                    //    }

                    //    break;

                    #endregion
                    #region Discipline

                    //case EnumFormName.Discipline:
                    //    this.radGridView1.AllowAddNewRow = false;
                    //    this.radGridView1.AllowDeleteRow = false;
                    //    //this.radGridView1.AllowRowReorder = false;
                    //    this.radGridView1.EnableCustomSorting = false;
                    //    this.radGridView1.EnableSorting = false;

                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "DisciplineID",
                    //        IsVisible = false,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "Name",
                    //        HeaderText = "Key",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        ReadOnly = true,
                    //        //
                    //        MaxLength = 25,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "Department",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        ReadOnly = true,
                    //        //
                    //        MaxLength = 25,
                    //    });

                    //    var disciplines = proxy.ReadDisciplines(false)
                    //        .Where(w => w.VisibleFlag.Equals(true))
                    //        .ToList();
                    //    // Rows Setting
                    //    foreach (var discipline in disciplines)
                    //    {
                    //        this.radGridView1.Rows.Add(discipline.DisciplineID, discipline.Name, discipline.NameView);
                    //    }

                    //    break;

                    #endregion
                    #region ListTypeRowCount

                    //case EnumFormName.ListTypeRowCount:
                    //    // Columns Setting
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "ListTypeID",
                    //        IsVisible = false,
                    //        ReadOnly = true,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    //    {
                    //        Name = "NameView",
                    //        HeaderText = "ListType",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        ReadOnly = true,
                    //    });
                    //    this.radGridView1.Columns.Add(new GridViewDecimalColumn()
                    //    {
                    //        Name = "RowCount",
                    //        HeaderText = "RowCount",
                    //        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    //        TextAlignment = ContentAlignment.MiddleCenter,
                    //        //
                    //        //DataType = typeof(int),
                    //    });

                    //    var listTypeRowCounts = proxy.ReadListTypes(false);
                    //    //.Where(w => w.VisibleFlag.Equals(true))

                    //    foreach (var listType in listTypeRowCounts)
                    //    {
                    //        this.radGridView1.Rows.Add(listType.ListTypeID, listType.NameView, listType.RowCount);
                    //    }
                    //    break;

                    #endregion

                    //SP
                    #region NamingRuleCodeDynamic

                    ////case EnumFormName.NamingRuleCodeDynamic
                    ////:
                    ////    // Columns Setting
                    ////    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    ////    {
                    ////        Name = "NamingRuleCodeDynamicID",
                    ////        IsVisible = false,
                    ////        ReadOnly = true,
                    ////    });
                    ////    this.radGridView1.Columns.Add(new GridViewTextBoxColumn()
                    ////    {
                    ////        Name = "NameView",
                    ////        HeaderText = "ListType",
                    ////        HeaderTextAlignment = ContentAlignment.MiddleCenter,
                    ////        TextAlignment = ContentAlignment.MiddleCenter,
                    ////        ReadOnly = true,
                    ////    });

                    ////    var namingRuleCodeDynamics = proxy.ReadNamingRuleCodeDynamics(
                    ////        this._entityFrmShareRadGridView.Project.ProjectID,
                    ////        this._entityFrmShareRadGridView.ListType.ListTypeID,
                    ////        this._entityFrmShareRadGridView.NamingRuleCodeDynamicIndex);
                    ////    //.Where(w => w.VisibleFlag.Equals(true))

                    ////    foreach (var namingRuleCodeDynamic in namingRuleCodeDynamics)
                    ////    {
                    ////        this.radGridView1.Rows.Add(namingRuleCodeDynamic.NamingRuleCodeDynamicID, namingRuleCodeDynamic.NameView);
                    ////    }
                    ////    break;

                    #endregion
                    //default:
                    //    break;
                } // end switch
                this.radGridView1.EndUpdate();
            }
        }

        void radDropDownListProject_SelectedIndexChanging(object sender, PositionChangingCancelEventArgs e)
        {
            var newProject = (EntityEDB_Project)this.radDropDownListProject.Items[e.Position].DataBoundItem;
            var oldProject = (EntityEDB_Project)this.radDropDownListProject.SelectedItem.DataBoundItem;
            if (!newProject.ListTypeFlag.Contains(this._entityEDBInformation.ListType.ListTypeID))
            {
                //프로젝트가 잘못선택되어있으면 아예 빠져나오질 못함.
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

            this.radDropDownListProject.ListElement.Font = new Font("Tahoma", (float)8.25, FontStyle.Regular);
            this.radDropDownListListType.ListElement.Font = new Font("Tahoma", (float)8.25, FontStyle.Regular);

            this.radGridView1.SelectionMode = GridViewSelectionMode.FullRowSelect;
            this.radGridView1.MultiSelect = true;
            //
            this.radGridView1.EnableGrouping = false;
            this.radGridView1.AllowCellContextMenu = false;
        }

        private void MethodSaveShareRadGridView()
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                bool flagSaveWhether = true;
                int? basicTemplateID = null;
                int? specificTemplateID = null;
                for (int i = 0; i < this.radGridView1.RowCount; i++)
                {
                    for (int j = 1; j < this.radGridView1.ColumnCount; j++) // j가 1부터 시작되는 이유 : 첫번째 Column 제외 (ID값은 추가시 자동으로 부여되기 때문에 없어야 함)
                    {
                        if (this.radGridView1.Columns[j].IsVisible
                            && !this.radGridView1.Columns[j].Name.Equals("EquipmentTypeDynamicID")
                            && !this.radGridView1.Columns[j].Name.Equals("NameKor")
                            && !this.radGridView1.Columns[j].Name.Equals("NameEng")
                            && !this.radGridView1.Columns[j].Name.Equals("PWD")
                            && !this.radGridView1.Columns[j].Name.Equals("PowerEquipmentProjectType")
                            && !this.radGridView1.Columns[j].Name.Equals("Capacity")
                            && !this.radGridView1.Columns[j].Name.Equals("PowerEquipmentNational")
                            && !this.radGridView1.Columns[j].Name.Equals("PrimaryFuelType")
                            && !this.radGridView1.Columns[j].Name.Equals("CoolingWaterSource")
                            && !this.radGridView1.Columns[j].Name.Equals("CreationDate")
                            && !this.radGridView1.Columns[j].Name.Equals("ClosingDate")
                            && !this.radGridView1.Columns[j].Name.Equals("Type2")
                            && !this.radGridView1.Columns[j].Name.Equals("Remark")
                            && !this.radGridView1.Columns[j].Name.Equals("Nde")
                            && !this.radGridView1.Columns[j].Name.Equals("CommonUnitID")
                                && !this.radGridView1.Columns[j].Name.Equals("FilePath")
                            ) //필드 제외
                        {
                            if ((this.radGridView1.Rows[i].Cells[j].Value == null || string.IsNullOrWhiteSpace(this.radGridView1.Rows[i].Cells[j].Value.ToString())) && !(this.radGridView1.Rows[i].Cells[j].ColumnInfo is GridViewCheckBoxColumn))
                            {
                                //flagSaveWhether = false;
                                //break;
                                new CustomRadMessageBoxShowAndTheme("Please fill in all the details without blanks.").RadMessageBoxShow();
                                return;
                            }
                        }
                    } // end for
                } // end for

                //if (flagSaveWhether)
                {
                    switch (this._entityTagNode.EnumFormName)
                    {
                        #region CaseDynamic
                        case BSL.Enums.EnumFormName.CaseDynamic:

                            if (this.radGridView1.Rows.Count < 1)
                            {
                                new CustomRadMessageBoxShowAndTheme("Data Is Empty").RadMessageBoxShow();
                                return;
                            }
                            var selectedProjectID = Convert.ToInt32(this.radDropDownListProject.SelectedValue);
                            var selectedListTypeID = Convert.ToInt32(this.radDropDownListListType.SelectedValue);
                            var caseDynamics = new List<EDB_CaseDynamic>();
                            var insertCaseDynamic = new List<int>();

                            SharedMethod.MethodCreateName(this.radGridView1, "Name", "NameView", true);
                            foreach (var row in this.radGridView1.Rows.OrderBy(ob => ob.Index))
                            {
                                var caseDynamic = new EDB_CaseDynamic();
                                caseDynamic.CaseDynamicID = Convert.ToInt32(row.Cells["CaseDynamicID"].Value);
                                caseDynamic.ProjectID = selectedProjectID;
                                caseDynamic.ListTypeID = selectedListTypeID;
                                caseDynamic.Orders = (row.Index + 1) * 100;
                                caseDynamic.Name = row.Cells["Name"].Value != null ? row.Cells["Name"].Value.ToString().Trim() : null;
                                caseDynamic.NameView = row.Cells["NameView"].Value != null ? row.Cells["NameView"].Value.ToString().Trim() : null;
                                if (this._entityEDBInformation.ListType.ListTypeID.Equals(4))
                                {
                                    caseDynamic.PackageFlag = row.Cells["PackageFlag"].Value.ToString().Equals("False") ? false : true;
                                }
                                //caseDynamic.VisibleFlag = row.Cells["Visible"].Value != null ? Convert.ToBoolean(row.Cells["Visible"].Value) : true;
                                //caseDynamic.EquipmentTypeDynamicID = row.Cells["EquipmentTypeDynamicID"].Value != null ? (int?)Convert.ToInt32(row.Cells["EquipmentTypeDynamicID"].Value) : null;
                                //DH
                                //caseDynamic.VisibleFlag = row.Cells["Visible"].Value != null ? Convert.ToBoolean(row.Cells["Visible"].Value) : false;
                                if (_entityEDBInformation.ListType.ListTypeID.Equals(ListTypeInstrumentProcessData))
                                {
                                    basicTemplateID = (int)radDropDownListBasicTemplate.SelectedItem.Tag;
                                    specificTemplateID = (int)radDropDownListSpecificTemplate.SelectedItem.Tag;
                                    caseDynamic.InstrumentBasicTemplateID = basicTemplateID;
                                    caseDynamic.InstrumentSpecificTemplateID = specificTemplateID;
                                }
                                caseDynamics.Add(caseDynamic);

                                if (caseDynamic.CaseDynamicID.Equals(0))
                                {
                                    insertCaseDynamic.Add(row.Index);
                                }
                            }

                            if (caseDynamics.Where(w => string.IsNullOrWhiteSpace(w.NameView)).Count() > 0)
                            {
                                new CustomRadMessageBoxShowAndTheme("Please fill in all the details without blanks.").RadMessageBoxShow();
                                return;
                            }

                            if (!this.radGridView1.RowCount.Equals(caseDynamics.GroupBy(gb => gb.Name.ToUpper()).Count()))
                            {
                                new CustomRadMessageBoxShowAndTheme("It is duplicated.").RadMessageBoxShow();
                                return;
                            }
                            if (!this.radGridView1.RowCount.Equals(caseDynamics.GroupBy(gb => gb.NameView.ToUpper()).Count()))
                            {
                                new CustomRadMessageBoxShowAndTheme("It is duplicated.").RadMessageBoxShow();
                                return;
                            }

                            if (caseDynamics.Where(w => new Regex("[\\\\\\*\\?\\/\\:\\[\\]]").IsMatch(w.NameView)).Count() > 0)
                            {
                                new CustomRadMessageBoxShowAndTheme("Characters you can't use in Sheet names \n \\ / : * ? [ ]").RadMessageBoxShow();
                                return;
                            }

                            if (_entityEDBInformation.ListType.ListTypeID.Equals(ListTypeInstrumentProcessData))
                            {
                            }

                            var oldCaseDynamics = proxy.ReadCaseDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, selectedProjectID, selectedListTypeID);

                            proxy.CreateCaseDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref caseDynamics, selectedProjectID, selectedListTypeID, null, true, basicTemplateID, specificTemplateID);

                            #region 새로운 시트의 컬럼 기본값 처리
                            var selectedProject = proxy.ReadProject(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, selectedProjectID);
                            var columnDynamicParents = proxy.ReadSimpleColumnDynamicParents(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, selectedProjectID, selectedListTypeID);


                            //EquipmentList
                            if (selectedListTypeID.Equals(4)
                                || selectedListTypeID.Equals(5)
                                || selectedListTypeID.Equals(11) // Electricalequipemnt
                                || selectedListTypeID.Equals(3)
                                || selectedListTypeID.Equals(7) // PEDB
                                || selectedListTypeID.Equals(8) // DataLinkSheet
                                || selectedListTypeID.Equals(9) // EquipmentDataBase
                                )
                            {
                                //Electrical 은 Parent 가 여러종류이므로 첫번째 Parent 를 불러와서 적용.
                                var readColumnDynamicParentDefault = proxy.ReadColumnDynamicParentDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.ListTypeID.Equals(selectedListTypeID)).FirstOrDefault();
                                var columnDynamicDefaultsCase = proxy.ReadColumnDynamicDefaultsByVisibleFlag(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.ListTypeID.Equals(selectedListTypeID));
                                for (int j = 0; j < insertCaseDynamic.Count; j++)
                                {
                                    int caseDynamicID = caseDynamics[insertCaseDynamic[j]].CaseDynamicID;
                                    //새로운 프로젝트의 경우 해당하는 ColumnDynamicParent 가 없기 떄문에 만들어 줌
                                    EDB_ColumnDynamicParent createColumnDynamicParent = new EDB_ColumnDynamicParent()
                                    {
                                        CaseDynamicID = caseDynamicID,
                                        ColumnHeaderMergeProperty = readColumnDynamicParentDefault != null ? readColumnDynamicParentDefault.ColumnHeaderMergeProperty : null,
                                    };
                                    proxy.CreateColumnDynamicParent(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref  createColumnDynamicParent, selectedProjectID, selectedListTypeID, null);
                                    var columnDynamics = new List<EDB_ColumnDynamic>();
                                    if (columnDynamicDefaultsCase.Count() > 0)
                                    {
                                        //foreach (var columnDynamicDefaultCase in columnDynamicDefaultsCase.Where(w => w.CaseDynamicDefaultName == 0 || w.CaseDynamicDefault == 100))
                                        foreach (var columnDynamicDefaultCase in columnDynamicDefaultsCase.GroupBy(gb => gb.CaseDynamicDefaultName).ElementAt(0))
                                        {
                                            EDB_ColumnDynamic columnDynamic = new EDB_ColumnDynamic();
                                            columnDynamic.ColumnIndex = columnDynamicDefaultCase.ColumnIndex;
                                            columnDynamic.CommonUnitID = columnDynamicDefaultCase.CommonUnitID;
                                            columnDynamic.VisibleFlag = true;
                                            columnDynamic.Orders = columnDynamicDefaultCase.Orders;
                                            columnDynamic.Label = columnDynamicDefaultCase.Label;
                                            columnDynamic.LabelView = columnDynamicDefaultCase.LabelView;
                                            columnDynamic.CellType = columnDynamicDefaultCase.CellType;
                                            columnDynamic.Locked = columnDynamicDefaultCase.Locked;
                                            columnDynamic.Width = columnDynamicDefaultCase.Width;
                                            columnDynamic.ColumnDynamicParentID = createColumnDynamicParent.ColumnDynamicParentID;
                                            columnDynamics.Add(columnDynamic);

                                        } // end foreach
                                    }
                                    else
                                    {
                                        EDB_ColumnDynamic columnDynamic = new EDB_ColumnDynamic();
                                        columnDynamic.ColumnIndex = 0;
                                        columnDynamic.CommonUnitID = 1;
                                        columnDynamic.VisibleFlag = true;
                                        columnDynamic.Orders = 100;
                                        columnDynamic.Label = "Column1";
                                        columnDynamic.LabelView = "Column1";
                                        columnDynamic.CellType = 0;
                                        columnDynamic.Locked = false;
                                        columnDynamic.ColumnDynamicParentID = createColumnDynamicParent.ColumnDynamicParentID;
                                        columnDynamics.Add(columnDynamic);
                                    }
                                    //COlumndynamic 에 데이터 추가
                                    proxy.CreateColumnDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, columnDynamics, selectedProjectID, selectedListTypeID, caseDynamicID, null);
                                }
                                var attributeSync = new EDCS.WF.BSL.Helpers.ExcelTemplateAttributeSyncController(this._entityEDBInformation.Project, this._entityEDBInformation.ListType);
                                attributeSync.MethodSyncTemplateAll();
                                attributeSync.MethodSyncTemplateDefault();
                                attributeSync.MethodSyncReportMappingHeader(caseDynamics, oldCaseDynamics);
                            }
                            else
                            {
                                //var caseDynamic = proxy.ReadCaseDynamics(false, selectedProjectID, selectedListTypeID).OrderBy(ob => ob.Orders).First();
                                var caseDynamic = caseDynamics.OrderBy(ob => ob.Orders).First();
                                int caseDynamicID = caseDynamic.CaseDynamicID;
                                if (!columnDynamicParents.Count.Equals(0))
                                {
                                    var columnDynamicParent = columnDynamicParents.First();

                                    EDB_ColumnDynamicParent createColumnDynamicParent = new EDB_ColumnDynamicParent()
                                    {
                                        ColumnDynamicParentID = columnDynamicParent.ColumnDynamicParentID,
                                        CaseDynamicID = caseDynamic.CaseDynamicID,
                                        DeleteFlag = columnDynamicParent.DeleteFlag,
                                        VisibleFlag = columnDynamicParent.VisibleFlag,
                                        ModifiedDate = columnDynamicParent.ModifiedDate,
                                        ColumnHeaderMergeProperty = columnDynamicParent.ColumnHeaderMergeProperty
                                    };

                                    proxy.CreateColumnDynamicParent(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref createColumnDynamicParent, selectedProjectID, selectedListTypeID, null);

                                }
                                else
                                {
                                    EDB_ColumnDynamicParent createColumnDynamicParent = new EDB_ColumnDynamicParent()
                                    {
                                        CaseDynamicID = caseDynamicID,
                                    };
                                    proxy.CreateColumnDynamicParent(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref createColumnDynamicParent, selectedProjectID, selectedListTypeID, null);

                                    var columnDynamicDefaultsCase = proxy.ReadColumnDynamicDefaultsByVisibleFlag(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.ListTypeID.Equals(selectedListTypeID));
                                    var columnDynamics = new List<EDB_ColumnDynamic>();

                                    if (columnDynamicDefaultsCase.Count() > 0)
                                    {
                                        foreach (var columnDynamicDefaultCase in columnDynamicDefaultsCase)
                                        {
                                            EDB_ColumnDynamic columnDynamic = new EDB_ColumnDynamic();
                                            columnDynamic.ColumnIndex = columnDynamicDefaultCase.ColumnIndex;
                                            columnDynamic.CommonUnitID = columnDynamicDefaultCase.CommonUnitID;
                                            columnDynamic.Orders = columnDynamicDefaultCase.Orders;
                                            columnDynamic.Label = columnDynamicDefaultCase.Label;
                                            columnDynamic.LabelView = columnDynamicDefaultCase.LabelView;
                                            columnDynamic.CellType = columnDynamicDefaultCase.CellType;
                                            columnDynamic.Locked = columnDynamicDefaultCase.Locked;
                                            columnDynamic.Width = columnDynamicDefaultCase.Width;
                                            columnDynamic.ColumnDynamicParentID = createColumnDynamicParent.ColumnDynamicParentID;
                                            columnDynamics.Add(columnDynamic);

                                        }// end foreach
                                    }
                                    else
                                    {
                                        EDB_ColumnDynamic columnDynamic = new EDB_ColumnDynamic();
                                        columnDynamic.ColumnIndex = 0;
                                        columnDynamic.CommonUnitID = 1;
                                        columnDynamic.Orders = 100;
                                        columnDynamic.Label = "Column1";
                                        columnDynamic.LabelView = "Column1";
                                        columnDynamic.CellType = 0;
                                        columnDynamic.Locked = false;
                                        columnDynamic.ColumnDynamicParentID = createColumnDynamicParent.ColumnDynamicParentID;
                                        columnDynamics.Add(columnDynamic);
                                    }
                                    //COlumndynamic 에 데이터 추가
                                    //proxy.CreateColumnDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, columnDynamics, selectedProjectID, selectedListTypeID, caseDynamicID, null);
                                }
                            }
                            #endregion

                            break;

                        #endregion
                        #region EquipmentTypeMapping

                        case BSL.Enums.EnumFormName.EquipmentTypeMappingMisc:
                        case BSL.Enums.EnumFormName.EquipmentTypeMappingPackage:
                            string caseDynamicName = "";
                            if (this._entityTagNode.EnumFormName == BSL.Enums.EnumFormName.EquipmentTypeMappingPackage) caseDynamicName = "pac";
                            else if (this._entityTagNode.EnumFormName == BSL.Enums.EnumFormName.EquipmentTypeMappingMisc) caseDynamicName = "mis";

                            var equipmentTypeDynamicMapping = new List<EDB_EquipmentTypeDynamic>();

                            foreach (var row in this.radGridView1.Rows.OrderBy(ob => ob.Index))
                            {
                                var powerEquipmentProjectType = new EDB_EquipmentTypeDynamic();
                                powerEquipmentProjectType.EquipmentTypeDynamicID = Convert.ToInt32(row.Cells["EquipmentTypeDynamicID"].Value);
                                //powerEquipmentProjectType.Orders = (row.Index + 1) * 100;
                                //powerEquipmentProjectType.CaseDynamicID = Convert.ToInt32(row.Cells["CaseDynamicID"].Value);
                                //powerEquipmentProjectType.NameView = row.Cells["NameView"].Value != null ? row.Cells["NameView"].Value.ToString().Trim() : "";
                                equipmentTypeDynamicMapping.Add(powerEquipmentProjectType);
                            }
                            this.MethodSaveEquipmentTypeMapping(equipmentTypeDynamicMapping, caseDynamicName);

                            // DB에 저장  
                            //proxy.CreateEquipmentTypeDynamic(powerEquipmentProjectTypes);
                            break;

                        #endregion
                        #region CaseDynamicDefault

                        case BSL.Enums.EnumFormName.CaseDynamicDefault:

                            var NewCaseDynamicDefaults = new List<EDB_CaseDynamicDefault>();
                            foreach (var row in this.radGridView1.Rows.OrderBy(ob => ob.Index))
                            {
                                var caseDynamicDefault = new EDB_CaseDynamicDefault();
                                caseDynamicDefault.CaseDynamicDefaultID = Convert.ToInt32(row.Cells["CaseDynamicDefaultID"].Value);
                                caseDynamicDefault.ListTypeID = Convert.ToInt32(this.radDropDownListListType.SelectedValue);
                                caseDynamicDefault.Orders = (row.Index + 1) * 100;
                                caseDynamicDefault.Name = row.Cells["Name"].Value != null ? row.Cells["Name"].Value.ToString().Trim() : null;
                                caseDynamicDefault.NameView = row.Cells["NameView"].Value != null ? row.Cells["NameView"].Value.ToString().Trim() : null;
                                caseDynamicDefault.VisibleFlag = row.Cells["Visible"].Value != null ? Convert.ToBoolean(row.Cells["Visible"].Value) : true;
                                if (this._entityEDBInformation.ListType.ListTypeID.Equals(4))
                                {
                                    caseDynamicDefault.PackageFlag = row.Cells["PackageFlag"].Value != null ? Convert.ToBoolean(row.Cells["PackageFlag"].Value) : true;
                                }
                                if (_entityEDBInformation.ListType.ListTypeID.Equals(ListTypeInstrumentProcessData))
                                {
                                    basicTemplateID = (int)radDropDownListBasicTemplate.SelectedItem.Tag;
                                    specificTemplateID = (int)radDropDownListSpecificTemplate.SelectedItem.Tag;
                                    caseDynamicDefault.InstrumentBasicTemplateID = basicTemplateID;
                                    caseDynamicDefault.InstrumentSpecificTemplateID = specificTemplateID;
                                }
                                //DH
                                NewCaseDynamicDefaults.Add(caseDynamicDefault);

                            }
                            proxy.CreateCaseDynamicDefault(EDCS.WF.BSL.Variable.SharedVariable.IsIE, NewCaseDynamicDefaults, Convert.ToInt32(this.radDropDownListListType.SelectedValue)
                                , basicTemplateID, specificTemplateID);

                            break;

                        #endregion
                        #region CommonColumnDynamic

                        case BSL.Enums.EnumFormName.CommonColumnDynamic:

                            var commonColumnDynamics = new List<EDB_CommonColumnDynamic>();

                            foreach (var row in this.radGridView1.Rows.OrderBy(ob => ob.Index))
                            {
                                var commonColumnDynamic = new EDB_CommonColumnDynamic();

                                commonColumnDynamic.CommonColumnDynamicID = Convert.ToInt32(row.Cells["CommonColumnDynamicID"].Value);
                                commonColumnDynamic.ListTypeID = Convert.ToInt32(this.radDropDownListListType.SelectedValue);
                                commonColumnDynamic.Orders = (row.Index + 1) * 100;
                                commonColumnDynamic.Name = row.Cells["Name"].Value != null ? row.Cells["Name"].Value.ToString().Trim() : null;
                                commonColumnDynamic.NameView = row.Cells["NameView"].Value != null ? row.Cells["NameView"].Value.ToString().Trim() : null;
                                commonColumnDynamic.CommonUnitID = row.Cells["CommonUnitID"].Value != null ? Convert.ToInt32(row.Cells["CommonUnitID"].Value) : 1;

                                commonColumnDynamics.Add(commonColumnDynamic);
                            }

                            proxy.CreateCommonColumnDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, commonColumnDynamics, Convert.ToInt32(this.radDropDownListListType.SelectedValue));

                            break;

                        #endregion
                        #region CommonSystemDynamic

                        case BSL.Enums.EnumFormName.CommonSystemDynamic:

                            var commonSystemDynamics = new List<EDB_CommonSystemDynamic>();

                            foreach (var row in this.radGridView1.Rows.OrderBy(ob => ob.Index))
                            {
                                var commonSystemDynamic = new EDB_CommonSystemDynamic();

                                commonSystemDynamic.CommonSystemDynamicID = Convert.ToInt32(row.Cells["CommonSystemDynamicID"].Value);
                                commonSystemDynamic.Orders = (row.Index + 1) * 100;
                                commonSystemDynamic.Name = row.Cells["NameView"].Value != null ? row.Cells["NameView"].Value.ToString().Trim() : null;
                                commonSystemDynamic.NameView = row.Cells["NameView"].Value != null ? row.Cells["NameView"].Value.ToString().Trim() : null;
                                commonSystemDynamics.Add(commonSystemDynamic);
                            }

                            proxy.CreateCommonSystemDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, commonSystemDynamics);

                            break;

                        #endregion
                        #region ReplaceDynamic
                        case BSL.Enums.EnumFormName.ReplaceDynamic:

                            selectedProjectID = Convert.ToInt32(this.radDropDownListProject.SelectedValue);
                            selectedListTypeID = Convert.ToInt32(this.radDropDownListListType.SelectedValue);
                            var replaceDynamics = new List<EDB_ReplaceDynamic>();

                            foreach (var row in this.radGridView1.Rows.OrderBy(ob => ob.Index))
                            {
                                var replaceDynamic = new EDB_ReplaceDynamic();

                                replaceDynamic.ReplaceID = Convert.ToInt32(row.Cells["ReplaceID"].Value);
                                replaceDynamic.ProjectID = selectedProjectID;
                                replaceDynamic.ListTypeID = selectedListTypeID;
                                replaceDynamic.CommonColumnTypeID = Convert.ToInt32(row.Cells["CommonColumnType"].Value);
                                replaceDynamic.Orders = (row.Index + 1) * 100;
                                replaceDynamic.Name = row.Cells["Name"].Value != null ? row.Cells["Name"].Value.ToString().Trim() : null;
                                replaceDynamic.NameView = row.Cells["NameView"].Value != null ? row.Cells["NameView"].Value.ToString().Trim() : null;

                                if (Convert.ToDouble(row.Cells["Value"].Value).Equals(0))
                                {
                                    //new CustomRadMessageBoxShowAndTheme(Resources.Message10048).RadMessageBoxShow();
                                    //return;
                                }
                                replaceDynamic.Value = Convert.ToDouble(row.Cells["Value"].Value);
                                replaceDynamics.Add(replaceDynamic);
                            }

                            proxy.CreateReplaceDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, replaceDynamics, selectedProjectID, selectedListTypeID);

                            break;


                        #endregion
                        #region ReplaceDynamicDefault
                        case BSL.Enums.EnumFormName.ReplaceDynamicDefault:

                            selectedListTypeID = Convert.ToInt32(this.radDropDownListListType.SelectedValue);
                            var replaceDynamicDefaults = new List<EDB_ReplaceDynamicDefault>();

                            foreach (var row in this.radGridView1.Rows.OrderBy(ob => ob.Index))
                            {
                                var replaceDynamicDefault = new EDB_ReplaceDynamicDefault();

                                replaceDynamicDefault.ReplaceDynamicDefaultID = Convert.ToInt32(row.Cells["ReplacedynamicDefaultID"].Value);
                                replaceDynamicDefault.ListTypeID = selectedListTypeID;
                                replaceDynamicDefault.CommonColumnTypeID = Convert.ToInt32(row.Cells["CommonColumnType"].Value);
                                replaceDynamicDefault.Orders = (row.Index + 1) * 100;
                                replaceDynamicDefault.Name = row.Cells["Name"].Value != null ? row.Cells["Name"].Value.ToString().Trim() : null;
                                replaceDynamicDefault.NameView = row.Cells["NameView"].Value != null ? row.Cells["NameView"].Value.ToString().Trim() : null;
                                replaceDynamicDefault.VisibleFlag = true;
                                replaceDynamicDefault.ModifiedDate = DateTime.Now;
                                replaceDynamicDefault.DeleteFlag = false;
                                if (Convert.ToDouble(row.Cells["Value"].Value).Equals(0))
                                {
                                    //new CustomRadMessageBoxShowAndTheme(Resources.Message10048).RadMessageBoxShow();
                                    //return;
                                }
                                replaceDynamicDefault.Value = Convert.ToDouble(row.Cells["Value"].Value);
                                replaceDynamicDefaults.Add(replaceDynamicDefault);
                            }

                            proxy.CreateReplaceDynamicDefault(EDCS.WF.BSL.Variable.SharedVariable.IsIE, replaceDynamicDefaults, selectedListTypeID);

                            break;


                        #endregion
                    }
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="p_type">0 - 전체 Visible, 1 - ListType Only, 2 - 전체 Hidden, 3 - 전체 hidden & searchtextbox visible</param>
        private void MethodSettingDropDownListVisible(byte p_type)
        {
            radLabelProjectType.Visible = false;
            radDropDownListProjectType.Visible = false;

            if (p_type == 0)
            {
                radPanelProject.Visible = true;
                radPanelListType.Visible = true;

                radPanelListType.Location = new Point(3, 34);
                radGridView1.Location = new Point(0, 64);

                //this.radButtonInsertRow.Location = new Point(this.radButtonInsertRow.Location.X, 34);
                //this.radButtonRemoveRow.Location = new Point(this.radButtonRemoveRow.Location.X, 34);
            }
            else if (p_type == 1)
            {
                radPanelProject.Visible = false;
                radPanelListType.Visible = true;
                radPanelListType.Location = new Point(3, 4);
                radGridView1.Location = new Point(0, 34);
                //this.radButtonInsertRow.Location = new Point(this.radButtonInsertRow.Location.X, 4);
                //this.radButtonRemoveRow.Location = new Point(this.radButtonRemoveRow.Location.X, 4);
            }
            else if (p_type == 2)
            {
                radPanelProject.Visible = false;
                radPanelListType.Visible = false;
                //this.radButtonInsertRow.Location = new Point(this.radButtonInsertRow.Location.X, 34);
                //this.radButtonRemoveRow.Location = new Point(this.radButtonRemoveRow.Location.X, 34);
                radGridView1.Location = new Point(0, 34);
            }
        }

        private void MethodSettingDropDownListVisibleInstrument()
        {
            if (_entityTagNode.EnumFormName.Equals(BSL.Enums.EnumFormName.CaseDynamicDefault)
                || _entityTagNode.EnumFormName.Equals(BSL.Enums.EnumFormName.CaseDynamic))
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
        }
        private void radDropDownListProject_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            this._entityEDBInformation.Project = this.radDropDownListProject.SelectedItem.DataBoundItem as EntityEDB_Project;
            this.MethodBindingRadGridView();
        }

        private void radDropDownListListType_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            this._entityEDBInformation.ListType = this.radDropDownListListType.SelectedItem.DataBoundItem as EntityEDB_ListType;
            this.MethodSettingDropDownListVisibleInstrument();
            this.MethodBindingRadGridView();
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


        //UpButton
        private void radButtonUp_Click(object sender, EventArgs e)
        {
            SharedMethod.MethodRadGridViewMoveRow(this.radGridView1, true);
        }
        //DownButton
        private void radButtonDown_Click(object sender, EventArgs e)
        {
            SharedMethod.MethodRadGridViewMoveRow(this.radGridView1, false);
        }
        //DMCSExtraction
        private void radButtonDMCSExtraction_Click(object sender, EventArgs e)
        {


        }

        private void MethodSaveEquipmentTypeMapping(List<EDB_EquipmentTypeDynamic> p_equipmentTypeDynamicMapping, string p_caseDynamicName)
        {
            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var caseDynamics = proxy.ReadCaseDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, Convert.ToInt32(this.radDropDownListProject.SelectedValue), Convert.ToInt32(this.radDropDownListListType.SelectedValue));
                var oldEquipmentTypeDynamics = proxy.ReadEquipmentTypeDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, Convert.ToInt32(this.radDropDownListProject.SelectedValue), Convert.ToInt32(this.radDropDownListListType.SelectedValue));
                //oldEquipmentTypeDynamics = oldEquipmentTypeDynamics.Where(w => w.ParentID == 0).ToList();

                var commonColumnDynamics = proxy.ReadCommonColumnDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, Convert.ToInt32(this.radDropDownListListType.SelectedValue));

                var insertEquipmentTypeDynamics = new List<EntityEDB_EquipmentTypeDynamic>();
                var deleteEquipmentTypeDynamics = new List<EntityEDB_EquipmentTypeDynamic>();
                var caseDynamic = caseDynamics.SingleOrDefault(s => s.Name == p_caseDynamicName);
                if (caseDynamic != null)
                {
                    // 추가
                    var insertEquipmentTypeDynamicIDs = p_equipmentTypeDynamicMapping.Select(s => s.EquipmentTypeDynamicID).Except(oldEquipmentTypeDynamics.Where(w => w.ParentID == 0 && w.CaseDynamicID == caseDynamic.CaseDynamicID).Select(s => s.EquipmentTypeDynamicID));
                    insertEquipmentTypeDynamics.AddRange(oldEquipmentTypeDynamics.Where(w => insertEquipmentTypeDynamicIDs.Contains(w.EquipmentTypeDynamicID) || insertEquipmentTypeDynamicIDs.Contains(w.ParentID)));
                    insertEquipmentTypeDynamics.ConvertAll(ca => ca.CaseDynamicID == caseDynamic.CaseDynamicID);

                    //MISC 에 추가
                    //EQuipmentType 의 정보 수정
                    //CAseDybamic 삭제
                    //추가된 equipment 의 기본컬럼 추가(기존 column 과 비교하여 없는것만)
                    StringBuilder sb = new StringBuilder();
                    var insertColumnDynamicNames = new List<string>();
                    EntityEDB_EquipmentTypeDynamicDefault equipmentTypeDefault = null;
                    foreach (var equipmentType in insertEquipmentTypeDynamics)
                    {
                        sb.AppendFormat("Update EDB_EQuipmentTypeDynamic set CaseDynamicID = {0} where EquipmentTypeDynamicID = {1};", caseDynamic.CaseDynamicID, equipmentType.EquipmentTypeDynamicID);
                        equipmentTypeDefault = this._entityEDBInformation.EquipmentTypeDefaults.SingleOrDefault(w => w.Name.Length < 5 && w.Name.Substring(0, 2) == equipmentType.GroupCode && w.Name.Substring(2, 2) == equipmentType.Name);
                        if (equipmentTypeDefault != null)
                        {
                            insertColumnDynamicNames.AddRange(this._entityEDBInformation.ColumnDynamicDefaults.Where(w => w.CaseDynamicDefaultName == equipmentTypeDefault.CaseDynamicName).Select(s => s.Label));
                        }
                    }
                    insertColumnDynamicNames = insertColumnDynamicNames.Distinct().ToList();
                    //if (insertColumnDynamicNames.Count > 0)
                    //{
                    //    var oldColumnDynamics = proxy.ReadColumnDynamics(false, caseDynamic.CaseDynamicID);
                    //    insertColumnDynamicNames = insertColumnDynamicNames.Except(oldColumnDynamics.Select(s => s.Label)).ToList();
                    //    foreach (var columnName in insertColumnDynamicNames)
                    //    {
                    //        var newColumnDynamic = new EDCS.WF.DSL.ColumnDynamic();
                    //        var commonColumnDynamic = commonColumnDynamics.SingleOrDefault(s => s.Name == columnName);
                    //        var startIndex = commonColumnDynamic.Orders - 100 / 100;
                    //        for (int i = startIndex; i > 0; i--)
                    //        {
                    //            if (oldColumnDynamics.Select(s => s.Label).Contains(commonColumnDynamics[i].Name))
                    //            {
                    //            }
                    //        }
                    //    }
                    //    //this._entityFrmShareRadGridView._columnDynamicDefaults.Where(w => w.CaseDynamicDefaultName == equipmentTypeDefault.CaseDynamicName);
                    //}
                    foreach (var modifyCaseDynamic in caseDynamics)
                    {
                        if (insertEquipmentTypeDynamics.Select(s => s.CaseDynamicID).Contains(modifyCaseDynamic.CaseDynamicID))
                        {
                            if (modifyCaseDynamic.Name != "pac" && modifyCaseDynamic.Name != "mis")
                            {
                                modifyCaseDynamic.DeleteFlag = true;
                                caseDynamics.Where(w => w.Orders > modifyCaseDynamic.Orders).ToList().ConvertAll(ca => ca.Orders -= 100);
                            }
                        }
                    }
                    if (sb.Length > 0)
                    {
                        proxy.ExecuteNonQuery(EDCS.WF.BSL.Variable.SharedVariable.IsIE, sb);
                        proxy.UpdateCaseDynamicOrdersDeleteFlag(EDCS.WF.BSL.Variable.SharedVariable.IsIE, caseDynamics);
                        caseDynamics = proxy.ReadCaseDynamics(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, Convert.ToInt32(this.radDropDownListProject.SelectedValue), Convert.ToInt32(this.radDropDownListListType.SelectedValue));
                    }
                    sb.Clear();


                    var caseDynamicDefaults = this._entityEDBInformation.CaseDynamicDefaults.Where(w => w.VisibleFlag == true && w.ListTypeID == Convert.ToInt32(this.radDropDownListListType.SelectedValue)).ToList();
                    var columnDynamicParentDefaults = proxy.ReadColumnDynamicParentDefaults(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false).Where(w => w.ListTypeID == Convert.ToInt32(this.radDropDownListListType.SelectedValue)).ToList();
                    var columnDynamicDefaults = this._entityEDBInformation.ColumnDynamicDefaults.Where(w => w.VisibleFlag == true && w.ListTypeID == Convert.ToInt32(this.radDropDownListListType.SelectedValue)).ToList();
                    var equipmentTypeDynamicDefaults = this._entityEDBInformation.EquipmentTypeDefaults.Where(w => w.VisibleFlag == true && w.ListTypeID == Convert.ToInt32(this.radDropDownListListType.SelectedValue)).ToList();

                    //Misc 에서 삭제
                    //EquipmentType 정보 수정
                    //CaseDynamic 추가
                    //Columndefault 추가.
                    //c삭제된 Equipemtn ㅣ의 기본컬럼 삭제(기존 column 과 비교하여 )
                    //삭제
                    var deleteEquipmentTypeDynamicIDs = oldEquipmentTypeDynamics.Where(w => w.ParentID == 0 && w.CaseDynamicID == caseDynamic.CaseDynamicID).Select(s => s.EquipmentTypeDynamicID).Except(p_equipmentTypeDynamicMapping.Select(s => s.EquipmentTypeDynamicID));
                    deleteEquipmentTypeDynamics.AddRange(oldEquipmentTypeDynamics.Where(w => deleteEquipmentTypeDynamicIDs.Contains(w.EquipmentTypeDynamicID)));
                    foreach (var equipmentType in deleteEquipmentTypeDynamics)
                    {
                        var equipmentTypeDynamicDefault = equipmentTypeDynamicDefaults.SingleOrDefault(w => w.Name.Length < 5 && w.Name.Substring(0, 2) == equipmentType.GroupCode && w.Name.Substring(2, 2) == equipmentType.Name);
                        if (equipmentTypeDynamicDefault != null)
                        {
                            var caseDynamicDefault = caseDynamicDefaults.SingleOrDefault(s => s.Name == equipmentTypeDynamicDefault.CaseDynamicInitialName);
                            if (caseDynamicDefault != null) //null 일수가 없음.
                            {
                                if (equipmentTypeDynamicDefault.CaseDynamicInitialName == "mis" || equipmentTypeDynamicDefault.CaseDynamicInitialName == "pac")
                                {
                                    var caseDynamicAll = proxy.ReadCaseDynamicsIncludeDelete(EDCS.WF.BSL.Variable.SharedVariable.IsIE, Convert.ToInt32(this.radDropDownListProject.SelectedValue), Convert.ToInt32(this.radDropDownListListType.SelectedValue));
                                    var intCaseDynamic = caseDynamicAll.SingleOrDefault(s => s.Name == "int");
                                    if (intCaseDynamic != null)
                                    {
                                        sb.AppendFormat("Update EDB_EQuipmentTypeDynamic set CaseDynamicID = {0} where EquipmentTypeDynamicID = {1};", intCaseDynamic.CaseDynamicID, equipmentType.EquipmentTypeDynamicID);
                                        foreach (var oldEquip in oldEquipmentTypeDynamics.Where(w => w.ParentID == equipmentType.EquipmentTypeDynamicID))
                                        {
                                            sb.AppendFormat("Update EDB_EQuipmentTypeDynamic set CaseDynamicID = {0} where EquipmentTypeDynamicID = {1};", intCaseDynamic.CaseDynamicID, oldEquip.EquipmentTypeDynamicID);
                                        }
                                        proxy.ExecuteNonQuery(EDCS.WF.BSL.Variable.SharedVariable.IsIE, sb);
                                        sb.Clear();
                                    }
                                }
                                else
                                {
                                    //들어갈 자리 구함(Orders)
                                    var newOrders = 100;
                                    if (caseDynamicDefault.Orders != 100)
                                    {
                                        var caseDynamicDefaultOrders = caseDynamicDefaults.SingleOrDefault(s => s.Orders == caseDynamicDefault.Orders - 100);
                                        var caseDynamicOrders = caseDynamics.SingleOrDefault(s => s.Name == caseDynamicDefaultOrders.Name);
                                        if (caseDynamicOrders == null) { newOrders = caseDynamics.Max(m => m.Orders) + 100; }
                                        else { newOrders = caseDynamicOrders.Orders + 100; }
                                    }
                                    ///
                                    //CAse 추가.
                                    var newCaseDynamic = new EDB_CaseDynamic();
                                    newCaseDynamic.Orders = newOrders;
                                    newCaseDynamic.ProjectID = Convert.ToInt32(this.radDropDownListProject.SelectedValue);
                                    newCaseDynamic.ListTypeID = Convert.ToInt32(this.radDropDownListListType.SelectedValue);
                                    newCaseDynamic.Name = caseDynamicDefault.Name;
                                    newCaseDynamic.NameView = caseDynamicDefault.NameView;
                                    caseDynamics.Where(w => w.Orders >= newOrders).ToList().ConvertAll(ca => ca.Orders += 100);

                                    proxy.UpdateCaseDynamicOrdersDeleteFlag(EDCS.WF.BSL.Variable.SharedVariable.IsIE, caseDynamics);
                                    proxy.CreateCaseDynamic(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newCaseDynamic);

                                    sb.AppendFormat("Update EDB_EQuipmentTypeDynamic set CaseDynamicID = {0} where EquipmentTypeDynamicID = {1};", newCaseDynamic.CaseDynamicID, equipmentType.EquipmentTypeDynamicID);
                                    foreach (var oldEquip in oldEquipmentTypeDynamics.Where(w => w.ParentID == equipmentType.EquipmentTypeDynamicID))
                                    {
                                        sb.AppendFormat("Update EDB_EQuipmentTypeDynamic set CaseDynamicID = {0} where EquipmentTypeDynamicID = {1};", newCaseDynamic.CaseDynamicID, oldEquip.EquipmentTypeDynamicID);
                                    }
                                    proxy.ExecuteNonQuery(EDCS.WF.BSL.Variable.SharedVariable.IsIE, sb);
                                    sb.Clear();

                                    var columnDynamicParentDefault = columnDynamicParentDefaults.SingleOrDefault(w => w.CaseDynamicDefaultName == caseDynamicDefault.Name);
                                    var newColumnDynamicParent = new EDB_ColumnDynamicParent();
                                    if (columnDynamicParentDefault != null)
                                    {
                                        newColumnDynamicParent.CaseDynamicID = newCaseDynamic.CaseDynamicID;
                                        newColumnDynamicParent.ColumnHeaderMergeProperty = columnDynamicParentDefault.ColumnHeaderMergeProperty;
                                        proxy.CreateColumnDynamicParent(EDCS.WF.BSL.Variable.SharedVariable.IsIE, ref newColumnDynamicParent, 0, 0, null, true);

                                        var columnDynamicDefault = columnDynamicDefaults.Where(w => w.CaseDynamicDefaultName == caseDynamicDefault.Name);
                                        if (columnDynamicDefault.Count() > 0)
                                        {
                                            var newColumnDynamics = new List<EDB_ColumnDynamic>();
                                            foreach (var columnDefault in columnDynamicDefault)
                                            {
                                                EDB_ColumnDynamic columnDynamic = new EDB_ColumnDynamic();
                                                columnDynamic.ColumnIndex = columnDefault.ColumnIndex;
                                                columnDynamic.CommonUnitID = columnDefault.CommonUnitID;
                                                columnDynamic.Orders = columnDefault.Orders;
                                                columnDynamic.Label = columnDefault.Label;
                                                columnDynamic.LabelView = columnDefault.LabelView;
                                                columnDynamic.CellType = columnDefault.CellType;
                                                columnDynamic.Locked = columnDefault.Locked;
                                                columnDynamic.Width = columnDefault.Width;
                                                columnDynamic.ColumnDynamicParentID = newColumnDynamicParent.ColumnDynamicParentID;
                                                newColumnDynamics.Add(columnDynamic);
                                            }
                                            proxy.CreateColumnDynamicInsertOnly(EDCS.WF.BSL.Variable.SharedVariable.IsIE, newColumnDynamics);
                                        }
                                    }
                                }
                            }
                        }

                        //sb.AppendFormat("Update EQuipmentTypeDynamic set CaseDynamicID = {0} where EquipmentTypeDynamicID = {1};", caseDynamic.CaseDynamicID, equipmentType.EquipmentTypeDynamicID);
                    }
                }
            }
        }

        private void radDropDownListBasicTemplate_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
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
                        else
                            projectType = this._entityEDBInformation.Project.Type2 != null ? this._entityEDBInformation.Project.Type2.Value : 1;


                        var InstrumentSpecificTemplates = proxy.ReadInstrumentSpecificTemplate(EDCS.WF.BSL.Variable.SharedVariable.IsIE, false, basicTemplateID, projectType);
                        radDropDownListSpecificTemplate.SelectedIndexChanged -= new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListSpecificTemplate_SelectedIndexChanged);
                        radDropDownListSpecificTemplate.Items.Clear();
                        foreach (var InstrumentSpecificTemplate in InstrumentSpecificTemplates)
                        {
                            RadListDataItem item = new RadListDataItem();
                            item.Tag = InstrumentSpecificTemplate.InstrumentSpecificTemplateID;
                            item.Text = InstrumentSpecificTemplate.NameView;
                            radDropDownListSpecificTemplate.Items.Add(item);
                        }
                        radDropDownListSpecificTemplate.SelectedIndexChanged += new Telerik.WinControls.UI.Data.PositionChangedEventHandler(radDropDownListSpecificTemplate_SelectedIndexChanged);
                        radDropDownListSpecificTemplate.SelectedIndex = 0;
                    }
                }
            }

        }

        private void radDropDownListSpecificTemplate_SelectedIndexChanged(object sender, Telerik.WinControls.UI.Data.PositionChangedEventArgs e)
        {
            MethodBindingRadGridView();
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
