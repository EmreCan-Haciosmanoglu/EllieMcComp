<%@ Page Title="Detail" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Detail.aspx.cs" Inherits="WebApp.Detail" %>


<asp:Content ID="BodyContent" ContentPlaceHolderID="MainContent" runat="server">
    <link href="Detail.css" rel="stylesheet" />
    <div class="Detail_Div">
        <div class="Images">
            <div class="MainImage">
                <div class="Left">
                    <asp:ImageButton class="Left_Image" runat="server" />
                </div>
                <div class="Image_Div">
                    <asp:Image class="Image" runat="server" />
                </div>
                <div class="Right">
                    <asp:ImageButton class="Right_Image" runat="server" />
                </div>
            </div>
            <div class="ImagePrevs">

            </div>
        </div>
        <div class="Details">
            <div class="Title">

            </div>
            <div class="Description">

            </div>
            <div class="Buy">
                <div class="Price">

                </div>
                <div class="Button">

                </div>
            </div>
        </div>
    </div>
</asp:Content>
