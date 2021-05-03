using System;
using System.Collections.Generic;
using System.Windows.Forms;
using EDCS.WF.DSL;
using Telerik.WinControls.Enumerations;
using Telerik.WinControls.UI;

namespace Configuration
{
    public partial class UserControlFilter : UserControl
    {
        

        public UserControlFilter()
        {
            InitializeComponent();
            ToggleButton();
        }

        public void SetCboColumns(bool isIE)
        {
            
        }

        public void SetCboColumns(long lProjectID, string itemTypeName)
        {
            radDropDownListColumns.Items.Clear();
            if (itemTypeName.Trim().Length == 0)
            {
                AllControlOn(false);
                return;
            }
            else
            {
                AllControlOn(true);
            }

            var proxy = WcfHelper.CreateProxyNetTcp<IEdcsDac>(EDCS.WF.BSL.Variable.SharedVariable.IsIE);
            using (proxy as IDisposable)
            {
                var lstAttributes = proxy.ReadDataColumn(EDCS.WF.BSL.Variable.SharedVariable.IsIE, lProjectID, itemTypeName);
                foreach (var row in lstAttributes)
                {
                    RadListDataItem item = new RadListDataItem();
                    item.Text = row.ColumnName;
                    item.Font = Information.FONT_REGULAR;
                    radDropDownListColumns.Items.Add(item);
                }
            }
            radDropDownListColumns.SortStyle = SortStyle.Ascending;
        }

        public void SetRadListDataItem(RadListDataItemCollection collection)
        {
            radDropDownListColumns.Items.Clear();
            radDropDownListColumns.Items.AddRange(collection);
        }

        public RadListDataItemCollection GetRadListDataItem()
        {
            return radDropDownListColumns.Items;           
        }

        public void InitCboColumns()
        {
            radDropDownListColumns.Items.Clear();
        }

        public string GetFilters()
        {
            return radTextBoxFilters.Text;
        }

        public void SetFilters(string filterValue)
        {
            radTextBoxFilters.Text = filterValue;
        }

        private void AllControlOn(bool vValue)
        {
            radDropDownListColumns.Enabled = vValue;
            radDropDownListOperator.Enabled = vValue;
            radTextBoxValue.Enabled = vValue;
            radButtonStartBracket.Enabled = vValue;
            radButtonEndBracket.Enabled = vValue;
            radTextBoxFilters.Enabled = vValue;
            radButtonAllClear.Enabled = vValue;
        }

        private void radButtonAndAdd_Click(object sender, EventArgs e)
        {
            string strValue = radTextBoxValue.Text;
            strValue = strValue.Replace("'", "");

            string sColumnName = radDropDownListColumns.Text;

            string sValue = radTextBoxFilters.Text.Trim();
            if (!string.IsNullOrEmpty(sValue))
            {
                if ((sValue.Substring(sValue.Length - 3)) != "AND")
                {
                    if ((sValue.Substring(sValue.Length - 1)) == "(")
                    {
                        radTextBoxFilters.Text = sValue.Substring(0, sValue.Length - 1);
                        sColumnName = "AND ( " + sColumnName;
                    }
                    else
                    { 
                    sColumnName = "\r\nAND " + sColumnName;
                    }
                }

                
            }

            //if (radTextBoxFilters.Text.Trim().Length <= 0)
            //{
            //    if (radDropDownListOperator.Text.ToUpper().Trim() == "IS NULL" || radDropDownListOperator.Text.ToUpper().Trim() == "IS NOT NULL")
            //        radTextBoxFilters.Text = sColumnName + " " + radDropDownListOperator.Text + " ";                
            //    else
            //        radTextBoxFilters.Text = sColumnName + " " + radDropDownListOperator.Text + " '" + strValue + "' ";
            //}
            //else
            //{
                if (radDropDownListOperator.Text.ToUpper().Trim() == "IS NULL" || radDropDownListOperator.Text.ToUpper().Trim() == "IS NOT NULL")
                    radTextBoxFilters.Text += sColumnName + " " + radDropDownListOperator.Text + " ";                
                else
                    radTextBoxFilters.Text += sColumnName + " " + radDropDownListOperator.Text + " '" + strValue + "' ";
            //}
        }

        private void radButtonAddOr_Click(object sender, EventArgs e)
        {
            string strValue = radTextBoxValue.Text;
            strValue = strValue.Replace("'", "");

            string sColumnName = radDropDownListColumns.Text;

            string sValue = radTextBoxFilters.Text.Trim();
            if (!string.IsNullOrEmpty(sValue))
            {
                if ((sValue.Substring(sValue.Length - 2)) != "OR")
                {
                    if ((sValue.Substring(sValue.Length - 1)) == "(")
                    {
                        radTextBoxFilters.Text = sValue.Substring(0, sValue.Length - 1);
                        sColumnName = "OR ( " + sColumnName;
                    }
                    else
                    {
                        sColumnName = "\r\nOR " + sColumnName;
                    }
                }
            }

            //if (radTextBoxFilters.Text.Trim().Length <= 0)
            //{
            //    if (radDropDownListOperator.Text.ToUpper().Trim() == "IS NULL" || radDropDownListOperator.Text.ToUpper().Trim() == "IS NOT NULL")
            //        radTextBoxFilters.Text = sColumnName + " " + radDropDownListOperator.Text + " ";
            //    else
            //        radTextBoxFilters.Text = sColumnName + " " + radDropDownListOperator.Text + " '" + strValue + "' ";
            //}
            //else
            //{
                if (radDropDownListOperator.Text.ToUpper().Trim() == "IS NULL" || radDropDownListOperator.Text.ToUpper().Trim() == "IS NOT NULL")
                    radTextBoxFilters.Text += sColumnName + " " + radDropDownListOperator.Text + " ";
                else
                    radTextBoxFilters.Text += sColumnName + " " + radDropDownListOperator.Text + " '" + strValue + "' ";
            //}
        }

        private void radButtonStartBracket_Click(object sender, EventArgs e)
        {
            //string sValue = radTextBoxFilters.Text.Trim();

            //if ((sValue.Substring(sValue.Length-3)) == "AND")
            //{
            //    radTextBoxFilters.Text += "\r\n(";

            //}
            //if ((sValue.Substring(sValue.Length - 2)) == "OR")
            //{
            //    radTextBoxFilters.Text += "\r\n(";

            //}

            if (radTextBoxFilters.Text.Trim().Length > 0)
                radTextBoxFilters.Text += "\r\n(";
            else
                radTextBoxFilters.Text += "(";
        }

        private void radButtonEndBracket_Click(object sender, EventArgs e)
        {
            if (radTextBoxFilters.Text.Trim().Length >= 0)
                radTextBoxFilters.Text += ")";
        }

        private void radDropDownListColumns_TextChanged(object sender, EventArgs e)
        {
            radTextBoxValue.Text = string.Empty;
            ToggleButton();
        }

        private void radDropDownListOperator_TextChanged(object sender, EventArgs e)
        {
            ToggleButton();
        }

        private void radTextBoxValue_TextChanged(object sender, EventArgs e)
        {
            ToggleButton();
        }

        private void ToggleButton()
        {
            //bool bEnabled = false;

            //if (radDropDownListColumns.Text.Trim() == "" || radDropDownListOperator.Text.Trim() == "" || 
            //    radTextBoxValue.Text.Trim() == "")
            //{
            //    if (radDropDownListColumns.Text.Trim() != "" &&
            //        (radDropDownListOperator.Text.ToUpper().Trim() == "IS NULL" || radDropDownListOperator.Text.ToUpper().Trim() == "IS NOT NULL"))                
            //        bEnabled = true;                
            //    else
            //        bEnabled = false;
            //}
            //else
            //{
            //    bEnabled = true;
            //}

            //radButtonAndAdd.Enabled = bEnabled;
            //radButtonAddOr.Enabled = bEnabled;
        }

        private void radButtonAllClear_Click(object sender, EventArgs e)
        {
            radTextBoxFilters.Text = string.Empty;
        }

        private void kradButtonAddANDNewLine_Click(object sender, EventArgs e)
        {
            if (radTextBoxFilters.Text.Trim().Length > 0)
                radTextBoxFilters.Text += "\r\nAND ";
            //else
            //    radTextBoxFilters.Text += "(";
        }

        private void kradButtonAddORNewLine_Click(object sender, EventArgs e)
        {
            if (radTextBoxFilters.Text.Trim().Length > 0)
                radTextBoxFilters.Text += "\r\nOR ";
        }

    }
}
