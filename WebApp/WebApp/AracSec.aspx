<%@ Page Title="Araç Seçimi" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="AracSec.aspx.cs" Inherits="WebApp.AracSec" %>


<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">
    <link href="GeneralSS.css" rel="stylesheet" />
    <div>
        <div id="ID_AracSec_Div">
            <div class="Make">
                <asp:DropDownList CssClass="DDL_AracSec" ID="Make" runat="server" AutoPostBack="true" OnSelectedIndexChanged="Make_SelectedIndexChanged">
                    <asp:ListItem Value="0">Marka Seçin</asp:ListItem>
                </asp:DropDownList>
            </div>
            <div class="Model">
                <asp:DropDownList CssClass="DDL_AracSec" ID="Model" runat="server" Enabled="true" AutoPostBack="True" OnSelectedIndexChanged="Model_SelectedIndexChanged">
                    <asp:ListItem Value="0" Text="Model Seç" />
                </asp:DropDownList>
            </div>
            <div class="Year">
                <asp:DropDownList CssClass="DDL_AracSec" ID="Year" runat="server" Enabled="true" AutoPostBack="true" OnSelectedIndexChanged="Year_SelectedIndexChanged">
                    <asp:ListItem Value="0" Text="Yılı Seçin" />
                </asp:DropDownList>
            </div>
            <div class="Trim">
                <asp:DropDownList CssClass="DDL_AracSec" ID="Trim" runat="server" Enabled="true" AutoPostBack="True" OnSelectedIndexChanged="Trim_SelectedIndexChanged">
                    <asp:ListItem Value="0" Text="Paketi Seçin" />
                </asp:DropDownList>
            </div>
            <div class="Envater">
                <asp:Button CssClass="Button_Envanter" ID="Envanter" runat="server" OnClick="Envanter_Click" Text="Tamamla >>" />
            </div>
        </div>
    </div>
</asp:Content>
