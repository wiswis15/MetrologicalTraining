<?xml version="1.0" encoding='ISO-8859-1' standalone='yes'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" xml:lang="en">

<xsl:template match="/">
<HTML>
<HEAD>
<TITLE>Test Report</TITLE>
<STYLE>
TABLE		    { color:#222222; font-size:10pt; font-family:'Arial' 'sanserif'; }
TH		        { font-weight:normal; color:#FFFFFF; background-color:#888888; }
TR.check	    { background-color:#EEEEEE }
TD.check	    { background-color:#EEEEEE }
H1		{ color:#111111; font-family:'Times New Roman' 'Arial' 'serif'; border-style:solid; border-width:0px; border-bottom-width:3px; border-bottom-color:#982E3C; }
H2		{ color:#222222; font-family:'Times New Roman' 'Arial' 'serif'; border-style:solid; border-width:0px; border-bottom-width:2px; border-bottom-color:#982E3C; }
H3		{ color:#333333; font-family:'Times New Roman' 'Arial' 'serif'; border-style:solid; border-width:0px; border-bottom-width:1px; border-bottom-color:#0E1B8D; margin-bottom:8px; }
H4		{ color:#444444; font-family:'Times New Roman' 'Arial' 'serif'; border-style:solid; border-width:0px; border-bottom-width:1px; border-bottom-color:#CCCCDD; margin-bottom:8px; }
H5		{ color:#555555; font-family:'Times New Roman' 'Arial' 'serif'; border-style:solid; border-width:0px; border-bottom-width:1px; border-bottom-color:#EEEEFF; margin-bottom:8px; }
H6		{ color:#666666; font-family:'Times New Roman' 'Arial' 'serif'; border-style:solid; border-width:0px; border-bottom-width:1px; border-bottom-color:#F8F8FF; margin-bottom:8px; }
SPAN.good		{ color:#006666; font-weight:bold; }
SPAN.critical	{ color:#880000; font-weight:bold; }
SPAN.project	{ color:#982E3C; font-weight:bold; }
SPAN.projItalic	{ color:#AAAAAA; font-weight:bold; font-style:italic; }
</STYLE>
</HEAD>

<BODY>
  	<H1>Test Report</H1>
    	<xsl:apply-templates select="/TestRun/*"/>
</BODY>
</HTML>
</xsl:template>

<xsl:template match="TestSuiteInfo">
	<TABLE> 
		<TR>
		    <TD>
			<SPAN class="project">
				Unit test run of
				<SPAN class="projItalic">
					<xsl:value-of select="Project"/>
				</SPAN>
				started at:
			</SPAN>
			<xsl:value-of select="Date"/>
		   </TD>
		</TR>
	</TABLE>
</xsl:template>

<xsl:template match="FailedTest">
	<TR>
	<TD align="right" valign='top'><xsl:value-of select="@id"/></TD>
	<TD valign='top'><xsl:apply-templates select="Name"/></TD>
	<TD valign='top'><xsl:apply-templates select="FailureType"/></TD>
	<TD valign='top'><xsl:apply-templates select="Location"/></TD>
	<TD valign='top'><pre><xsl:apply-templates select="Message"/></pre></TD>
	</TR>
</xsl:template>


<xsl:template match="FailedTests">
    <H2>Failed Tests</H2>
    <xsl:choose>
	<xsl:when test="FailedTest">
		<TABLE>
			<TR>
				<TH> ID </TH>
				<TH>Name</TH>
				<TH>FailureType</TH>
				<TH>Location</TH>
				<TH>Message</TH>
			</TR>
		  	<xsl:apply-templates select="FailedTest"/>
		</TABLE>
	</xsl:when>
	<xsl:otherwise>
	    <SPAN class="good">No failed test.</SPAN>
	</xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template match="FailedTest">
	<TR>
	<TD align="right" valign='top'><xsl:value-of select="@id"/></TD>
	<TD valign='top'><xsl:apply-templates select="Name"/></TD>
	<TD valign='top'><xsl:apply-templates select="FailureType"/></TD>
	<TD valign='top'><xsl:apply-templates select="Location"/></TD>
	<TD valign='top'><pre><xsl:apply-templates select="Message"/></pre></TD>
	</TR>
</xsl:template>

<xsl:template match="Name|FailureType|Message"><xsl:value-of select="."/></xsl:template>

<xsl:template match="Location">
	<xsl:if test=".">
		<xsl:value-of select="File"/>(<xsl:value-of select="Line"/>)
	</xsl:if>
</xsl:template>
  
<xsl:template match="SuccessfulTests">
    <H2>Successful Tests</H2>
    <xsl:choose>
	<xsl:when test="Test">
	<TABLE>
		<TR>
			<TH>&#160;ID&#160;</TH>
			<TH>Name</TH>
		</TR>
	  	<xsl:apply-templates select="Test"/>
	</TABLE>
	</xsl:when>
	<xsl:otherwise>
	    <SPAN class="critical">No successful test.</SPAN>
	</xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template match="Test">
	<TR>
		<TD align="right"><xsl:value-of select="@id"/></TD>
		<TD><xsl:apply-templates select="Name"/></TD>
	</TR>
</xsl:template>

<xsl:template match="Statistics">
    <H2>Statistics</H2>
	<TABLE>
		<TR>
			<TH>Status</TH>
			<TH>Number</TH>
		</TR>

		<TR>
			<TD>Tests</TD>
			<TD align="center"><xsl:value-of select="Tests"/></TD>
		</TR>

		<TR>
			<TD>Failed Tests</TD>
			<TD align="center"><xsl:value-of select="FailuresTotal"/></TD>
		</TR>

		<TR>
			<TD align="right">Errors</TD>
			<TD align="center"><xsl:value-of select="Errors"/></TD>
		</TR>

		<TR>
			<TD align="right">Failures</TD>
			<TD align="center"><xsl:value-of select="Failures"/></TD>
		</TR>
		
	</TABLE>
</xsl:template>

</xsl:stylesheet>
