<%@ Page Title="Araç Seçimi" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="AracSec.aspx.cs" Inherits="WebApp.AracSec" %>


<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">
    <link href="GeneralSS.css" rel="stylesheet" />
    <div>
        <div class="Make">
            <asp:DropDownList ID="Make" runat="server" AutoPostBack="true" OnSelectedIndexChanged="Make_SelectedIndexChanged">
                <asp:ListItem Value="0">Marka Seçin</asp:ListItem>
                <asp:ListItem Value="1">Ford</asp:ListItem>
                <asp:ListItem Value="2">Tesla</asp:ListItem>
            </asp:DropDownList>
        </div>
        <div class="Model">
            <asp:DropDownList ID="Model" runat="server" AutoPostBack="True" OnSelectedIndexChanged="Model_SelectedIndexChanged">
                <asp:ListItem Value="0">Model Seç</asp:ListItem>
            </asp:DropDownList>
        </div>
    </div>
</asp:Content>
