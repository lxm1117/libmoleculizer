<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet exclude-result-prefixes="xalan" version="1.0" xmlns:rng="http://relaxng.org/ns/structure/1.0" xmlns:xalan="http://xml.apache.org/xalan" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"><xsl:output method="html"/><xsl:param name="caption"/><xsl:template match="/"><xsl:element name="html"><xsl:element name="head"><xsl:element name="title"><xsl:value-of select="$caption"/></xsl:element></xsl:element><xsl:element name="body"><xsl:element name="H1"><xsl:value-of select="$caption"/></xsl:element><xsl:element name="table"><xsl:attribute name="border"/><xsl:element name="tr"><xsl:element name="th">Element</xsl:element><xsl:element name="th">Context</xsl:element></xsl:element><xsl:variable name="elt-anchor-items-fragment"><xsl:apply-templates select="rng:grammar-doc/rng:start"><xsl:with-param name="xpth" select="'/'"/></xsl:apply-templates></xsl:variable><xsl:variable name="elt-anchor-items" select="xalan:nodeset($elt-anchor-items-fragment)"/><xsl:for-each select="$elt-anchor-items/*"><xsl:sort select="td[1]/a/text()"/><xsl:copy-of select="."/></xsl:for-each></xsl:element></xsl:element></xsl:element></xsl:template><xsl:template match="*"><xsl:param name="xpth"/><xsl:apply-templates select="*"><xsl:with-param name="xpth" select="$xpth"/></xsl:apply-templates></xsl:template><xsl:template match="rng:element"><xsl:param name="xpth"/><xsl:variable name="curpth" select="concat($xpth, @name)"/><xsl:element name="tr"><xsl:element name="td"><xsl:element name="a"><xsl:attribute name="href"><xsl:call-template name="xpth-name"><xsl:with-param name="xpth" select="$curpth"/><xsl:with-param name="rest-pth" select="substring-after($curpth, '/')"/></xsl:call-template></xsl:attribute><xsl:value-of select="@name"/></xsl:element></xsl:element><xsl:element name="td"><xsl:call-template name="decorated-path-context"><xsl:with-param name="pth" select="$curpth"/></xsl:call-template></xsl:element></xsl:element><xsl:apply-templates select="*"><xsl:with-param name="xpth" select="concat($curpth, '/')"/></xsl:apply-templates></xsl:template><xsl:template match="rng:ref"><xsl:param name="xpth"/><xsl:variable name="ref-name" select="@name"/><xsl:apply-templates select="/rng:grammar-doc/rng:define[@name = $ref-name]"><xsl:with-param name="xpth" select="$xpth"/></xsl:apply-templates></xsl:template><xsl:template name="path-anchor-text"><xsl:param name="pth"/><xsl:choose><xsl:when test="contains($pth, '/')"><xsl:call-template name="path-anchor-text"><xsl:with-param name="pth" select="substring-after($pth, '/')"/></xsl:call-template></xsl:when><xsl:otherwise><xsl:value-of select="$pth"/></xsl:otherwise></xsl:choose></xsl:template><xsl:template name="path-context"><xsl:param name="pth"/><xsl:choose><xsl:when test="string-length(translate($pth, translate($pth, '/', ''), '')) &gt; 3"><xsl:call-template name="path-context"><xsl:with-param name="pth" select="substring-after($pth, '/')"/></xsl:call-template></xsl:when><xsl:otherwise><xsl:value-of select="$pth"/></xsl:otherwise></xsl:choose></xsl:template><xsl:template name="decorated-path-context"><xsl:param name="pth"/><xsl:choose><xsl:when test="string-length(translate($pth, translate($pth, '/', ''), '')) &gt; 3"><xsl:text>... </xsl:text><xsl:call-template name="path-context"><xsl:with-param name="pth" select="substring-after($pth, '/')"/></xsl:call-template></xsl:when><xsl:otherwise><xsl:value-of select="$pth"/></xsl:otherwise></xsl:choose></xsl:template><xsl:template name="xpth-name"><xsl:param name="xpth"/><xsl:param name="rest-pth"/><xsl:variable name="cur-pth" select="substring-before($xpth, concat('/', $rest-pth))"/><xsl:variable name="next-rest-pth" select="substring-after($rest-pth, '/')"/><xsl:if test="$cur-pth"><xsl:text>_</xsl:text><xsl:call-template name="path-anchor-text"><xsl:with-param name="pth" select="$cur-pth"/></xsl:call-template></xsl:if><xsl:choose><xsl:when test="$next-rest-pth"><xsl:call-template name="xpth-name"><xsl:with-param name="xpth" select="$xpth"/><xsl:with-param name="rest-pth" select="$next-rest-pth"/></xsl:call-template></xsl:when><xsl:otherwise><xsl:text>_</xsl:text><xsl:call-template name="path-anchor-text"><xsl:with-param name="pth" select="$xpth"/></xsl:call-template><xsl:text>.html</xsl:text></xsl:otherwise></xsl:choose></xsl:template></xsl:stylesheet>