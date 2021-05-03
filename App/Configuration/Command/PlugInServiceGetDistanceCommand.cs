using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Net.Sockets;
using System.IO;

namespace Configuration
{
	public class PlugInServiceGetDistanceCommand : IDisposable
	{
		const int Port = 4421;
		public string ControlName{ get; set; }
        
        public double Distance{ get; set; }

		public void Dispose()
		{
			GC.SuppressFinalize(this);
		}

		protected virtual void Dispose(bool disposing)
		{
		}

		private DataSet GetCommandParam()
		{
			DataSet dsParam = null;

			try
			{
				dsParam = new DataSet();
				{
					DataTable dtParam = new DataTable("Command");
					{
						dtParam.Columns.Add(new DataColumn("Name", typeof(string)));
					}
					dsParam.Tables.Add(dtParam);
				}
				{
					DataTable dtParam = new DataTable("Params");
					{
						dtParam.Columns.Add(new DataColumn("ControlName", typeof(string)));
						dsParam.Tables.Add(dtParam);
					}
				}
									
				DataRow drParam = dsParam.Tables["Command"].NewRow();
				{
					drParam["Name"] = "GetDistance";
					dsParam.Tables["Command"].Rows.Add(drParam);
				}
				drParam = dsParam.Tables["Params"].NewRow();
				{
					drParam["ControlName"] = ControlName;
					dsParam.Tables["Params"].Rows.Add(drParam);
				}
			}
			catch { }
			finally { }

			return dsParam;
		}

		public bool Execute()
		{
			try
			{
				using (TcpClient tcpClient = new TcpClient("127.0.0.1", Port))
				{
					using (NetworkStream stream = tcpClient.GetStream())
					{
						DataSet dsParam = this.GetCommandParam();
						if (null != dsParam)
						{
							StringWriter sw = new StringWriter();
							dsParam.WriteXml(sw);
							byte[] oDataSize = BitConverter.GetBytes(sw.ToString().Length);
							byte[] buffer = Encoding.UTF8.GetBytes(sw.ToString());
							byte[] oResult = new byte[oDataSize.Length + buffer.Length];
							System.Buffer.BlockCopy(oDataSize, 0, oResult, 0, oDataSize.Length);
							System.Buffer.BlockCopy(buffer, 0, oResult, oDataSize.Length, buffer.Length);
							stream.Write(oResult, 0, oResult.Length);
							stream.Flush();

                            /// get result from AutoCAD
                            buffer = new byte[4];
                            stream.Read(buffer, 0, buffer.Length);
                            Int32 iDataSize = BitConverter.ToInt32(buffer, 0);
                            buffer = new byte[iDataSize];
                            stream.Read(buffer, 0, iDataSize);
                            string sResult = Encoding.UTF8.GetString(buffer);
                            DataSet dsResult = new DataSet();
                            dsResult.ReadXml(new StringReader(sResult));
                            /// check result
                            if ((1 == dsResult.Tables.Count) && (1 == dsResult.Tables[0].Rows.Count))
                            {
                                try
                                {
                                    Distance = Convert.ToDouble(dsResult.Tables[0].Rows[0]["Value"].ToString());
                                    return true; 
                                }
                                finally { }
                            }
						}
					}
				}
			}
			catch (System.Exception ex)
			{
				throw ex;
			}

			return false;
		}
	}
}
