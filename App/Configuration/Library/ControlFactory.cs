using System.Drawing;

namespace Configuration
{
    public class ControlFactory
    {
        public static UserControlBase Create(string name)
        {
            switch (name)
            {
                case "UserControlCommonProject":
                    var userControlCommonProject = new UserControlCommonProject();
                    return userControlCommonProject;
                //case "UserControlCommonEmployee":
                //    var userControlCommonEmployee = new UserControlCommonEmployee();
                //    return userControlCommonEmployee;
                //case "UserControlCommonAttributeSetting":
                //    var userControlSPPIDAttributeSetting = new UserControlSPPIDAttributeSetting();
                //    return userControlSPPIDAttributeSetting;
                //case "UserControlEDBShareRadGridView":
                //    var userControlShareRadGridView = new UserControlEDBShareRadGridView();
                //    userControlShareRadGridView.Size = new Size(700, 500);
                //    return userControlShareRadGridView;
                //case "UserControlEDBCopyDefault":
                //    var userControlCopyDefault = new UserControlEDBCopyDefault();
                //    return userControlCopyDefault;
                //case "UserControlEDBCopyProject":
                //    var userControlCopyProject = new UserControlEDBCopyProject();
                //    return userControlCopyProject;
                //case "UserControlEDBDistrictType":
                //    var userControlDistrictType = new UserControlEDBDistrictType();
                //    return userControlDistrictType;
                //case "UserControlEDBBQColumnMapping":
                //    var userControlBQColumnMapping = new UserControlEDBBQColumnMapping();
                //    return userControlBQColumnMapping;
                //case "UserControlEDBUOM":
                //    var userControlEDBUOM = new UserControlEDBUOM();
                //    return userControlEDBUOM;
                //case "UserControlEDBAttributeSetting":
                //    var userControlEDBAttributeSetting = new UserControlEDBAttributeSetting();
                //    return userControlEDBAttributeSetting;
                //case "UserControlEDBAttributeDefaultSetting":
                //    var userControlEDBAttributeDefaultSetting = new UserControlEDBAttributeDefaultSetting();
                //    return userControlEDBAttributeDefaultSetting;
                //case "UserControlEDBTagNamingRule":
                //    var userControlEDBTagNamingRule = new UserControlEDBTagNamingRule();
                //    return userControlEDBTagNamingRule;
                //case "UserControlEDBValidation":
                //    var userControlEDBValidation = new UserControlEDBValidation();
                //    return userControlEDBValidation;
                //case "UserControlEDBValidationDefault":
                //    var userControlEDBValidationDefault = new UserControlEDBValidationDefault();
                //    return userControlEDBValidationDefault;
                ////case "UserControlPowerEquipmentService":
                ////    var userControlPowerEquipmentService = new UserControlPowerEquipmentService();
                ////    return userControlPowerEquipmentService;
                ////case "UserControlSystemMapping":
                ////    var userControlSystemMapping = new UserControlSystemMapping();
                ////    return userControlSystemMapping;
                //case "UserControlSPPIDDrawingList":
                //    var userControlSPPIDDrawingList = new UserControlSPPIDDrawingList();
                //    return userControlSPPIDDrawingList;
                //case "UserControlCommonTagRuleSetting":
                //    var userControlCommonTagRuleSetting = new UserControlCommonTagRuleSetting();
                //    return userControlCommonTagRuleSetting;
                //case "UserControlCommonPBSMappingSetting":
                //    var userControlCommonPBSMappingSetting = new UserControlCommonPBSMappingSetting();
                //    return userControlCommonPBSMappingSetting;
                //case "UserControlCommonAttributeMapping":
                //    var userControlCommonAttributeMapping = new UserControlCommonAttributeMapping();
                //    return userControlCommonAttributeMapping;
                //case "UserControlDocNoMapping":
                //    var userControlDocNoMapping = new UserControlDocNoMapping();
                //    return userControlDocNoMapping;
                //case "UserControlSPPIDMarkTagNoSetting":
                //    var userControlSPPIDMarkTagNoSetting = new UserControlSPPIDMarkTagNoSetting();
                //    return userControlSPPIDMarkTagNoSetting;
                //case "UserControlSPPIDPipingCompSetting":
                //    var userControlSPPIDPipingCompSetting = new UserControlSPPIDPipingCompSetting();
                //    return userControlSPPIDPipingCompSetting;
                //case "UserControlInstrumentDDPInhouseData":
                //    var userControlInstrumentDDPInhouseData = new UserControlInstrumentDDPInhouseData();
                //    return userControlInstrumentDDPInhouseData;
                //case "UserControlInstrumentIndexAttribute":
                //    var UserControlInstrumentIndexAttribute = new UserControlInstrumentIndexAttribute();
                //    return UserControlInstrumentIndexAttribute;
                //case "UserControlInstrumentTagClassMapping":
                //    var UserControlInstrumentTagClassMapping = new UserControlInstrumentTagClassMapping();
                //    return UserControlInstrumentTagClassMapping;
                //case "UserControlEDBInstrumentCataType":
                //    var UserControlEDBInstrumentCataType = new UserControlEDBInstrumentCataType();
                //    return UserControlEDBInstrumentCataType;
                //case "UserControlInstrumentDDPMapping":
                //    var UserControlInstrumentDDPMapping = new UserControlInstrumentDDPMapping();
                //    return UserControlInstrumentDDPMapping;
                //case "UserControlInstrumentProcessDataOwnership":
                //    var UserControlInstrumentProcessDataOwnership = new UserControlInstrumentProcessDataOwnership();
                //    return UserControlInstrumentProcessDataOwnership;
                //case "UserControlInstrumentBasicTemplate":
                //    var UserControlInstrumentBasicTemplate = new UserControlInstrumentBasicTemplate();
                //    return UserControlInstrumentBasicTemplate;

                //// SP3D
                //case "UserControlSP3DLineNoDisplayFormat":
                //    var userControlSP3DLineNoDisplayFormat = new UserControlSP3DLineNoDisplayFormat();
                //    return userControlSP3DLineNoDisplayFormat;


                /// added by humkyung - 2017.05.09 added by humkyung
                case UserControlSmartISOLinelistSetting.DisplayName:
                    return new UserControlSmartISOLinelistSetting();
                case UserControlSmartISOSiteSetting.DisplayName:
                    return new UserControlSmartISOSiteSetting();
                case UserControlPrjSetting.DisplayName:
                    return new UserControlPrjSetting();
                case UserControlBorderSetting.DisplayName:
                    return new UserControlBorderSetting();
                case UserControlRevSetting.DisplayName:
                    return new UserControlRevSetting();
                /// added by humkyung - 2017.02.10 added by yongs
                case UserControlIsoProductSetting.DisplayName:
                    return new UserControlIsoProductSetting();

                //case "UserControlCommonRolebyMenu":
                //    var userControlCommonRolebyMenu = new UserControlCommonRolebyMenu();
                //    return userControlCommonRolebyMenu;
                //case "UserControlCommonValidation":
                //    var userControlCommonValidation = new UserControlCommonValidation();
                //    return userControlCommonValidation;
                //case "UserControlCommonStandardRole":
                //    var userControlCommonStandardRole = new UserControlCommonStandardRole();
                //    return userControlCommonStandardRole;
                //case "UserControlCommonRole":
                //    var userControlCommonRole = new UserControlCommonRole();
                //    return userControlCommonRole;
                //case "UserControlCommonMenu":
                //    var userControlCommonMenu = new UserControlCommonMenu();
                //    return userControlCommonMenu;
                //case "UserControlCommonEmplyeeByProject":
                //    var userControlCommonEmplyeeByProject = new UserControlCommonEmplyeeByProject();
                //    return userControlCommonEmplyeeByProject;
                //case "UserControlSPPIDDataFilterSetting":
                //    var userControlSPPIDDataFilterSetting = new UserControlSPPIDDataFilterSetting();
                //    return userControlSPPIDDataFilterSetting;
                //case "UserControlSPPIDProjectInfoCopy":
                //    var userControlSPPIDProjectInfoCopy = new UserControlSPPIDProjectInfoCopy();
                //    return userControlSPPIDProjectInfoCopy;
                //case "UserControlCommonCreateData":
                //    var userControlCommonCreateData = new UserControlCommonCreateData();
                //    return userControlCommonCreateData;
                //case "UserControlNoData":
                //    var userControlNoData = new UserControlNoData();
                //    return userControlNoData;
                //case "UserControlCommonPBSSetting":
                //    var userControlCommonPBSSetting = new UserControlCommonPBSSetting();
                //    return userControlCommonPBSSetting;
                //case "UserControlSPPIDTagReadOnly":
                //    var userControlSPPIDTagReadOnly = new UserControlSPPIDTagReadOnly();
                //    return userControlSPPIDTagReadOnly;
                //case "UserControlCommonConnectLog":
                //    var userControlCommonConnectLog = new UserControlCommonConnectLog();
                //    return userControlCommonConnectLog;
                //case "UserControlSPPIDInstrumentCompareItemSetting":
                //    var userControlSPPIDInstrumentCompareItemSetting = new UserControlSPPIDInstrumentCompareItemSetting();
                //    return userControlSPPIDInstrumentCompareItemSetting;
                //case "UserControlCommonValidation_ItemTag":
                //    var userControlCommonValidation_ItemTag = new UserControlCommonValidation_ItemTag();
                //    return userControlCommonValidation_ItemTag;
                //case "UserControlShareFrmCell":
                //    var userControlShareFrmCell = new UserControlShareFrmCell();
                //    return userControlShareFrmCell;
                //case "UserControlShareFrmSplit":
                //    var userControlShareFrmSplit = new UserControlShareFrmSplit();
                //    return userControlShareFrmSplit;
                //case "UserControlShareFrmSplit_LIB":
                //    var userControlShareFrmSplit_LIB = new UserControlShareFrmSplit_LIB();
                //    return userControlShareFrmSplit_LIB;
            }
            // 없으면 Project로 리턴
            return null;
        }
    }
}
