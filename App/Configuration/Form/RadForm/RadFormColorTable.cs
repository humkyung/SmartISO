using TechSun.Theme;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Telerik.WinControls;

namespace Configuration
{
    public partial class RadFormColorTable : KRadForm
    {
        #region 전역 변수
        private static List<Color> lstcolor = new List<Color>();
        private string _ColorIndex = string.Empty;
        public string ColorIndex
        {
            get { return _ColorIndex; }
            set { _ColorIndex = value; }
        } 
        #endregion
                     
        #region GetRGBFromIndex
        public static Color GetRGBFromIndex(string index)
        {
            Color cColor = new Color();

            #region Set RGB Color
            if (0 == lstcolor.Count)
            {
                lstcolor.Add(Color.FromArgb(0, 0, 0));
                lstcolor.Add(Color.FromArgb(255, 0, 0));
                lstcolor.Add(Color.FromArgb(255, 255, 0));
                lstcolor.Add(Color.FromArgb(0, 255, 0));
                lstcolor.Add(Color.FromArgb(0, 255, 255));
                lstcolor.Add(Color.FromArgb(0, 0, 255));
                lstcolor.Add(Color.FromArgb(255, 0, 255));
                lstcolor.Add(Color.FromArgb(255, 255, 255));
                lstcolor.Add(Color.FromArgb(65, 65, 65));
                lstcolor.Add(Color.FromArgb(128, 128, 128));
                lstcolor.Add(Color.FromArgb(255, 0, 0));


                //11~20
                lstcolor.Add(Color.FromArgb(255, 170, 170));
                lstcolor.Add(Color.FromArgb(189, 0, 0));
                lstcolor.Add(Color.FromArgb(189, 126, 126));
                lstcolor.Add(Color.FromArgb(129, 0, 0));
                lstcolor.Add(Color.FromArgb(129, 86, 86));
                lstcolor.Add(Color.FromArgb(104, 0, 0));
                lstcolor.Add(Color.FromArgb(104, 69, 69));
                lstcolor.Add(Color.FromArgb(79, 0, 0));
                lstcolor.Add(Color.FromArgb(79, 53, 53));
                lstcolor.Add(Color.FromArgb(255, 63, 0));

                //21~30
                lstcolor.Add(Color.FromArgb(255, 191, 170));
                lstcolor.Add(Color.FromArgb(189, 46, 0));
                lstcolor.Add(Color.FromArgb(189, 141, 126));
                lstcolor.Add(Color.FromArgb(129, 31, 0));
                lstcolor.Add(Color.FromArgb(129, 96, 86));
                lstcolor.Add(Color.FromArgb(104, 25, 0));
                lstcolor.Add(Color.FromArgb(104, 78, 69));
                lstcolor.Add(Color.FromArgb(79, 19, 0));
                lstcolor.Add(Color.FromArgb(79, 59, 53));
                lstcolor.Add(Color.FromArgb(255, 127, 0));

                //31~40
                lstcolor.Add(Color.FromArgb(255, 212, 170));
                lstcolor.Add(Color.FromArgb(189, 94, 0));
                lstcolor.Add(Color.FromArgb(189, 157, 126));
                lstcolor.Add(Color.FromArgb(129, 64, 0));
                lstcolor.Add(Color.FromArgb(129, 107, 86));
                lstcolor.Add(Color.FromArgb(104, 52, 0));
                lstcolor.Add(Color.FromArgb(104, 86, 69));
                lstcolor.Add(Color.FromArgb(79, 39, 0));
                lstcolor.Add(Color.FromArgb(79, 66, 53));
                lstcolor.Add(Color.FromArgb(255, 191, 0));

                //41~50
                lstcolor.Add(Color.FromArgb(255, 234, 170));
                lstcolor.Add(Color.FromArgb(189, 141, 0));
                lstcolor.Add(Color.FromArgb(189, 173, 126));
                lstcolor.Add(Color.FromArgb(129, 96, 0));
                lstcolor.Add(Color.FromArgb(129, 118, 86));
                lstcolor.Add(Color.FromArgb(104, 78, 0));
                lstcolor.Add(Color.FromArgb(104, 95, 69));
                lstcolor.Add(Color.FromArgb(79, 59, 0));
                lstcolor.Add(Color.FromArgb(79, 73, 53));
                lstcolor.Add(Color.FromArgb(255, 255, 0));


                //51~60
                lstcolor.Add(Color.FromArgb(255, 255, 170));
                lstcolor.Add(Color.FromArgb(189, 189, 0));
                lstcolor.Add(Color.FromArgb(189, 189, 126));
                lstcolor.Add(Color.FromArgb(129, 129, 0));
                lstcolor.Add(Color.FromArgb(129, 129, 86));
                lstcolor.Add(Color.FromArgb(104, 104, 0));
                lstcolor.Add(Color.FromArgb(104, 104, 69));
                lstcolor.Add(Color.FromArgb(79, 79, 0));
                lstcolor.Add(Color.FromArgb(79, 73, 53));
                lstcolor.Add(Color.FromArgb(191, 255, 0));


                //61~70
                lstcolor.Add(Color.FromArgb(234, 255, 170));
                lstcolor.Add(Color.FromArgb(141, 189, 0));
                lstcolor.Add(Color.FromArgb(173, 189, 126));
                lstcolor.Add(Color.FromArgb(96, 129, 0));
                lstcolor.Add(Color.FromArgb(118, 129, 86));
                lstcolor.Add(Color.FromArgb(78, 104, 0));
                lstcolor.Add(Color.FromArgb(95, 104, 69));
                lstcolor.Add(Color.FromArgb(59, 79, 0));
                lstcolor.Add(Color.FromArgb(73, 79, 53));
                lstcolor.Add(Color.FromArgb(127, 255, 0));

                //71~80
                lstcolor.Add(Color.FromArgb(212, 255, 170));
                lstcolor.Add(Color.FromArgb(94, 189, 0));
                lstcolor.Add(Color.FromArgb(157, 189, 126));
                lstcolor.Add(Color.FromArgb(64, 129, 0));
                lstcolor.Add(Color.FromArgb(107, 129, 86));
                lstcolor.Add(Color.FromArgb(52, 104, 0));
                lstcolor.Add(Color.FromArgb(86, 104, 69));
                lstcolor.Add(Color.FromArgb(39, 79, 0));
                lstcolor.Add(Color.FromArgb(66, 79, 53));
                lstcolor.Add(Color.FromArgb(63, 255, 0));

                //81~90
                lstcolor.Add(Color.FromArgb(191, 255, 170));
                lstcolor.Add(Color.FromArgb(46, 189, 0));
                lstcolor.Add(Color.FromArgb(141, 189, 126));
                lstcolor.Add(Color.FromArgb(31, 129, 0));
                lstcolor.Add(Color.FromArgb(96, 129, 86));
                lstcolor.Add(Color.FromArgb(25, 104, 0));
                lstcolor.Add(Color.FromArgb(78, 104, 69));
                lstcolor.Add(Color.FromArgb(19, 79, 0));
                lstcolor.Add(Color.FromArgb(59, 79, 53));
                lstcolor.Add(Color.FromArgb(0, 255, 0));

                //91~100
                lstcolor.Add(Color.FromArgb(170, 255, 170));
                lstcolor.Add(Color.FromArgb(0, 189, 0));
                lstcolor.Add(Color.FromArgb(126, 189, 126));
                lstcolor.Add(Color.FromArgb(0, 129, 0));
                lstcolor.Add(Color.FromArgb(86, 129, 86));
                lstcolor.Add(Color.FromArgb(0, 104, 0));
                lstcolor.Add(Color.FromArgb(69, 104, 69));
                lstcolor.Add(Color.FromArgb(0, 79, 0));
                lstcolor.Add(Color.FromArgb(53, 79, 53));
                lstcolor.Add(Color.FromArgb(0, 255, 63));


                //101~110
                lstcolor.Add(Color.FromArgb(170, 255, 191));
                lstcolor.Add(Color.FromArgb(0, 189, 46));
                lstcolor.Add(Color.FromArgb(126, 189, 141));
                lstcolor.Add(Color.FromArgb(0, 129, 31));
                lstcolor.Add(Color.FromArgb(86, 129, 86));
                lstcolor.Add(Color.FromArgb(0, 104, 25));
                lstcolor.Add(Color.FromArgb(69, 104, 78));
                lstcolor.Add(Color.FromArgb(0, 79, 19));
                lstcolor.Add(Color.FromArgb(53, 79, 59));
                lstcolor.Add(Color.FromArgb(0, 255, 127));

                //111~120
                lstcolor.Add(Color.FromArgb(170, 255, 212));
                lstcolor.Add(Color.FromArgb(0, 189, 94));
                lstcolor.Add(Color.FromArgb(126, 189, 157));
                lstcolor.Add(Color.FromArgb(0, 129, 64));
                lstcolor.Add(Color.FromArgb(86, 129, 107));
                lstcolor.Add(Color.FromArgb(0, 104, 52));
                lstcolor.Add(Color.FromArgb(69, 104, 86));
                lstcolor.Add(Color.FromArgb(0, 79, 39));
                lstcolor.Add(Color.FromArgb(53, 79, 66));
                lstcolor.Add(Color.FromArgb(0, 255, 191));


                //121~130
                lstcolor.Add(Color.FromArgb(170, 255, 234));
                lstcolor.Add(Color.FromArgb(0, 189, 141));
                lstcolor.Add(Color.FromArgb(126, 189, 173));
                lstcolor.Add(Color.FromArgb(0, 129, 96));
                lstcolor.Add(Color.FromArgb(86, 129, 118));
                lstcolor.Add(Color.FromArgb(0, 104, 78));
                lstcolor.Add(Color.FromArgb(69, 104, 95));
                lstcolor.Add(Color.FromArgb(0, 79, 59));
                lstcolor.Add(Color.FromArgb(53, 79, 73));
                lstcolor.Add(Color.FromArgb(0, 255, 255));

                //131~140
                lstcolor.Add(Color.FromArgb(170, 255, 255));
                lstcolor.Add(Color.FromArgb(0, 189, 189));
                lstcolor.Add(Color.FromArgb(126, 189, 189));
                lstcolor.Add(Color.FromArgb(0, 129, 129));
                lstcolor.Add(Color.FromArgb(86, 129, 129));
                lstcolor.Add(Color.FromArgb(0, 104, 104));
                lstcolor.Add(Color.FromArgb(69, 104, 104));
                lstcolor.Add(Color.FromArgb(0, 79, 79));
                lstcolor.Add(Color.FromArgb(53, 79, 79));
                lstcolor.Add(Color.FromArgb(0, 191, 255));


                //141~150
                lstcolor.Add(Color.FromArgb(170, 234, 255));
                lstcolor.Add(Color.FromArgb(0, 141, 189));
                lstcolor.Add(Color.FromArgb(126, 173, 189));
                lstcolor.Add(Color.FromArgb(0, 96, 129));
                lstcolor.Add(Color.FromArgb(86, 118, 129));
                lstcolor.Add(Color.FromArgb(0, 78, 104));
                lstcolor.Add(Color.FromArgb(69, 95, 104));
                lstcolor.Add(Color.FromArgb(0, 59, 79));
                lstcolor.Add(Color.FromArgb(53, 73, 79));
                lstcolor.Add(Color.FromArgb(0, 127, 255));

                //151~160
                lstcolor.Add(Color.FromArgb(170, 212, 255));
                lstcolor.Add(Color.FromArgb(0, 94, 189));
                lstcolor.Add(Color.FromArgb(126, 157, 189));
                lstcolor.Add(Color.FromArgb(0, 64, 129));
                lstcolor.Add(Color.FromArgb(86, 107, 129));
                lstcolor.Add(Color.FromArgb(0, 52, 104));
                lstcolor.Add(Color.FromArgb(69, 86, 104));
                lstcolor.Add(Color.FromArgb(0, 39, 79));
                lstcolor.Add(Color.FromArgb(53, 66, 79));
                lstcolor.Add(Color.FromArgb(0, 63, 255));

                //161~170
                lstcolor.Add(Color.FromArgb(170, 191, 255));
                lstcolor.Add(Color.FromArgb(0, 46, 189));
                lstcolor.Add(Color.FromArgb(126, 141, 189));
                lstcolor.Add(Color.FromArgb(0, 31, 129));
                lstcolor.Add(Color.FromArgb(86, 96, 129));
                lstcolor.Add(Color.FromArgb(0, 25, 104));
                lstcolor.Add(Color.FromArgb(69, 78, 104));
                lstcolor.Add(Color.FromArgb(0, 19, 79));
                lstcolor.Add(Color.FromArgb(53, 59, 79));
                lstcolor.Add(Color.FromArgb(0, 0, 255));


                //171~180
                lstcolor.Add(Color.FromArgb(170, 170, 255));
                lstcolor.Add(Color.FromArgb(0, 0, 189));
                lstcolor.Add(Color.FromArgb(126, 126, 189));
                lstcolor.Add(Color.FromArgb(0, 0, 129));
                lstcolor.Add(Color.FromArgb(86, 86, 129));
                lstcolor.Add(Color.FromArgb(0, 0, 104));
                lstcolor.Add(Color.FromArgb(69, 69, 104));
                lstcolor.Add(Color.FromArgb(0, 0, 79));
                lstcolor.Add(Color.FromArgb(53, 53, 79));
                lstcolor.Add(Color.FromArgb(63, 0, 255));


                //181~190
                lstcolor.Add(Color.FromArgb(191, 170, 255));
                lstcolor.Add(Color.FromArgb(46, 0, 189));
                lstcolor.Add(Color.FromArgb(141, 126, 189));
                lstcolor.Add(Color.FromArgb(31, 0, 129));
                lstcolor.Add(Color.FromArgb(96, 86, 129));
                lstcolor.Add(Color.FromArgb(25, 0, 104));
                lstcolor.Add(Color.FromArgb(78, 69, 104));
                lstcolor.Add(Color.FromArgb(19, 0, 79));
                lstcolor.Add(Color.FromArgb(59, 53, 79));
                lstcolor.Add(Color.FromArgb(127, 0, 255));

                //191~200
                lstcolor.Add(Color.FromArgb(212, 170, 255));
                lstcolor.Add(Color.FromArgb(94, 0, 189));
                lstcolor.Add(Color.FromArgb(157, 126, 189));
                lstcolor.Add(Color.FromArgb(64, 0, 129));
                lstcolor.Add(Color.FromArgb(107, 86, 129));
                lstcolor.Add(Color.FromArgb(52, 0, 104));
                lstcolor.Add(Color.FromArgb(86, 69, 104));
                lstcolor.Add(Color.FromArgb(39, 0, 79));
                lstcolor.Add(Color.FromArgb(66, 53, 79));
                lstcolor.Add(Color.FromArgb(191, 0, 255));

                //201~210
                lstcolor.Add(Color.FromArgb(234, 170, 255));
                lstcolor.Add(Color.FromArgb(141, 0, 189));
                lstcolor.Add(Color.FromArgb(173, 126, 189));
                lstcolor.Add(Color.FromArgb(96, 0, 129));
                lstcolor.Add(Color.FromArgb(118, 86, 129));
                lstcolor.Add(Color.FromArgb(78, 0, 104));
                lstcolor.Add(Color.FromArgb(95, 69, 104));
                lstcolor.Add(Color.FromArgb(59, 0, 79));
                lstcolor.Add(Color.FromArgb(73, 53, 79));
                lstcolor.Add(Color.FromArgb(255, 0, 255));

                //211~220
                lstcolor.Add(Color.FromArgb(255, 170, 255));
                lstcolor.Add(Color.FromArgb(189, 0, 189));
                lstcolor.Add(Color.FromArgb(189, 126, 189));
                lstcolor.Add(Color.FromArgb(129, 0, 129));
                lstcolor.Add(Color.FromArgb(129, 86, 129));
                lstcolor.Add(Color.FromArgb(104, 0, 104));
                lstcolor.Add(Color.FromArgb(104, 69, 104));
                lstcolor.Add(Color.FromArgb(79, 0, 79));
                lstcolor.Add(Color.FromArgb(79, 53, 79));
                lstcolor.Add(Color.FromArgb(255, 0, 191));

                //221~230
                lstcolor.Add(Color.FromArgb(255, 170, 234));
                lstcolor.Add(Color.FromArgb(189, 0, 141));
                lstcolor.Add(Color.FromArgb(189, 126, 173));
                lstcolor.Add(Color.FromArgb(129, 0, 96));
                lstcolor.Add(Color.FromArgb(129, 86, 118));
                lstcolor.Add(Color.FromArgb(104, 0, 78));
                lstcolor.Add(Color.FromArgb(104, 69, 95));
                lstcolor.Add(Color.FromArgb(79, 0, 59));
                lstcolor.Add(Color.FromArgb(79, 53, 73));
                lstcolor.Add(Color.FromArgb(255, 0, 127));

                //231~240
                lstcolor.Add(Color.FromArgb(255, 170, 212));
                lstcolor.Add(Color.FromArgb(189, 0, 94));
                lstcolor.Add(Color.FromArgb(189, 126, 157));
                lstcolor.Add(Color.FromArgb(129, 0, 64));
                lstcolor.Add(Color.FromArgb(129, 86, 107));
                lstcolor.Add(Color.FromArgb(104, 0, 52));
                lstcolor.Add(Color.FromArgb(104, 69, 86));
                lstcolor.Add(Color.FromArgb(79, 0, 39));
                lstcolor.Add(Color.FromArgb(79, 53, 66));
                lstcolor.Add(Color.FromArgb(255, 0, 63));


                //241~250
                lstcolor.Add(Color.FromArgb(255, 170, 191));
                lstcolor.Add(Color.FromArgb(189, 0, 46));
                lstcolor.Add(Color.FromArgb(189, 126, 141));
                lstcolor.Add(Color.FromArgb(129, 0, 31));
                lstcolor.Add(Color.FromArgb(129, 86, 96));
                lstcolor.Add(Color.FromArgb(104, 0, 25));
                lstcolor.Add(Color.FromArgb(104, 69, 78));
                lstcolor.Add(Color.FromArgb(79, 0, 19));
                lstcolor.Add(Color.FromArgb(79, 53, 59));
                lstcolor.Add(Color.FromArgb(51, 51, 51));


                //251~255
                lstcolor.Add(Color.FromArgb(80, 80, 80));
                lstcolor.Add(Color.FromArgb(105, 105, 105));
                lstcolor.Add(Color.FromArgb(130, 130, 130));
                lstcolor.Add(Color.FromArgb(190, 190, 190));
                lstcolor.Add(Color.FromArgb(255, 255, 255));
            }
            #endregion

            #region Set Standard Color
            if (!System.Text.RegularExpressions.Regex.IsMatch(index.ToString(), "\\d+"))
            {
                if (index == "Black")
                    cColor = Color.Black;
                else if (index == "Red")
                    cColor = Color.Red;
                else if (index == "Yellow")
                    cColor = Color.Yellow;
                else if (index == "White")
                    cColor = Color.White;
                else if (index == "Green")
                    cColor = Color.Green;
                else if (index == "Cyan")
                    cColor = Color.Cyan;
                else if (index == "Blue")
                    cColor = Color.Blue;
                else if (index == "Magenta")
                    cColor = Color.Magenta;
            }
			else if (("0" == index) || ("256" == index))
			{
				cColor = Color.Transparent;
			}
			else
			{
				try
				{
					cColor = lstcolor[Convert.ToInt32(index)];
				}
				catch (Exception)
				{
					cColor = Color.Transparent;
				}
				
			}
            #endregion

            return cColor;
        }
        #endregion
               
        #region RadFormColorTable
        public RadFormColorTable()
        {
            InitializeComponent();           
        }
        #endregion             

        #region RadFormColorTable_Load
        private void RadFormColorTable_Load(object sender, EventArgs e)
        {
            #region InitColorTable

            int iColoridx = 0;
            int iLeft = 15;
            int iTop = 5;

            for (int i = 0; i < 256; i++ )
            {
                Label lblColor = new Label();
                lblColor.Name = "lblColor" + iColoridx.ToString();
                lblColor.BackColor = RadFormColorTable.GetRGBFromIndex(i.ToString());
              
                if ((iColoridx % 25) == 0)
                {
                    iLeft = 15;
                    iTop += 14 + 4;
                }

                lblColor.Left = iLeft;
                lblColor.Top = iTop;
                lblColor.Height = 13;
                lblColor.Width = 13;
                iLeft = lblColor.Left + lblColor.Width + 4;

                radGroupBoxFullColor.Controls.Add(lblColor);
                lblColor.Click += new EventHandler(lblColor_Click);

                iColoridx++;

            }
            #endregion

            #region Init Standard Color
            foreach (Control ocontrol in this.radGroupBoxStandardColor.Controls)
            {
                ocontrol.Click += new EventHandler(lblColor_Click);
            }
            #endregion

            #region Init Gray Color
            foreach (Control ocontrol in this.radGroupBoxGrayShades.Controls)
            {
                ocontrol.Click += new EventHandler(lblColor_Click);
            }  
            #endregion

            #region Init Defined & Current Color
            if(_ColorIndex != string.Empty)
            {
                radTextBoxDefineColorText.Text = _ColorIndex;
                radLabelDefineColor.BackColor = GetRGBFromIndex(_ColorIndex);
                 
                radTextBoxCurrentColor.Text = _ColorIndex;
                radLabelCurrentColor.BackColor = radLabelDefineColor.BackColor;   
            }
            #endregion

        }
        #endregion

        #region lblColor_Click
        private void lblColor_Click(object sender, EventArgs e)
        {
            Label olable = sender as Label;
            string sColorName = string.Empty;                            
            sColorName = olable.Name.Substring(8);  
                          
            radTextBoxCurrentColor.Text = sColorName;
            radLabelCurrentColor.BackColor = olable.BackColor;           
        }
        #endregion

        #region radButtonSelect_Click
        private void radButtonSelect_Click(object sender, EventArgs e)
        {
            ColorIndex = radTextBoxCurrentColor.Text;
            DialogResult = DialogResult.OK;
        }
        #endregion

        #region radButtonCancel_Click
        private void radButtonCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }
        #endregion

    }
}
