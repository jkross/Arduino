$areaFile = ".\V2D05WeeklyChanges.xml" 
$chartFile= ".\chartLatLon.xml"
$chartNum = 2770
$placeFile = ".\drift.xml"


function gll($chart = ".\V2D05WeeklyChanges.xml")
{
	[xml]$llxml = gc $chart
	$llre = (gc .\llnr.12270) -join "|"                                                                               
	$llxml.root.dataroot.Vol_x0020_2_x0020_D5_x0020_LL_x0020_corr_x0020_thru | where llnr -Match $llre | tee -Variable ll
	$ll | where Characteristic -NotMatch '^$' | tee -Variable lllighted                                               
}

function ParseLatLon($ll)
{
    if ($ll -match "(\d+)-(\d+)-(\d+(\.\d+)*)( *[NSEW])*") {
	   	$d=$matches[1]; $m=$matches[2]; $s=$matches[3]; 
		$dd = $d/1.0 + $m/60.0 + $s/(60.0*60.0); $dd
	} elseif ($ll -match "(\d+(\.\d+)*)( *[NSWE])*") {
		$dd = $matches[1] / 1.0; $dd
	} elseif ($ll -match "([-]*\d+(\.\d+)*)") {
		# only works if point is in same hemisphere as bounding box
		$dd = [math]::abs($matches[1] / 1.0); $dd
	} else {
		Write-Host ("{0}: does not parse" -f $ll)
		-1;
	}
}

function getBB($chartFile, $chartNum)
{
	[xml]$chart = gc $chartFile;
	$tc = $chart.charts.chart | where number -match $chartNum;
	$lats = @($tc.GetElementsByTagName("lat").'#text' | % { parseLatLon $_ } | sort) 
	$lons = @($tc.GetElementsByTagName("lon").'#text' | % { parseLatLon $_ } | sort) 
	nno @{lats = $lats; lons = $lons};
}

function isInRange($val, $range)
{
	if (($val -ge $range[0]) -and ($val -le $range[1])) {
		return $true;
	} else {
		return $false;
	}
}

function inBB($point, $bb)
{
	if ((isInRange $point.lat $bb.lats) -and (isInRange $point.lon $bb.lons)) {
		return $true;
	} else {
		return $false;
	}
}

function getInRange($areaFile, $chartFile, $chartNum)

{
	$bb = getBB $chartFile $chartNum;
	[xml]$areaXml = gc $areaFile;
	$mName = $areaXml.root.dataroot | Get-Member | where Name -Match Vol | select -ExpandProperty name 
	# $areaXml.root.dataroot.Vol_x0020_2_x0020_D5_x0020_LL_x0020_corr_x0020_thru | % {
	# $areaXml.root.dataroot.Vol_x0020_6_x0020_D13_x0020_LL_x0020_corr_x0020_thru | % {
	$areaXml.root.dataroot.$mName | % {
		$llat = ParseLatLon($_.Position_x0020__x0028_Latitude_x0029_);
		$llon = ParseLatLon($_.Position_x0020__x0028_Longitude_x0029_);
		nno @{lat = $llat; lon = $llon; v=$_}
	} | where -FilterScript { inBB $_ $bb } | 
	select -expandProperty v | 
	where Characteristic -notMatch '^$' | 
	% {
		$fix = $_;
		$fix.Structure = ($fix.Structure) -replace "\n",'::';
		$fix.Remarks = ($fix.Remarks) -replace "\n",'::';
		$fix;
	}
	

}

function AddDistance($light, $places)
{
		$llat = ParseLatLon($light.Position_x0020__x0028_Latitude_x0029_);
		$llon = ParseLatLon($light.Position_x0020__x0028_Longitude_x0029_);
		$drange = $light.Range / 1.0;
		$light | add-member -force -notepropertymembers @{drange=$drange;dlat=$llat;dlon=$llon;dlon_neg=-$llon};
		$minDist = 99999999;
		$places.places.place | % { 
			$name = "D_to_" + $_.name; 
			$plat = ParseLatLon($_.loc.lat);
			$plon = ParseLatLon($_.loc.lon);
			$dist = [math]::sqrt([math]::pow(($llat - $plat),2) + [math]::pow(($llon - $plon),2));
			if ($dist -lt $minDist) { $minDist = $dist };
			$light | add-member -force -notepropertymembers @{$name=$dist}
		}
		$light | add-member -force -notepropertymembers @{minDist=$minDist}
		$light
}

function LightSort($inRange, $placeFile)
{
	[xml]$places = gc $placefile;
	$inRange | % { AddDistance $_ $places } |
		sort -Property @{Expression="drange"; Descending=$true},@{Expression="minDist";Descending=$false} 
}

# $ir = getInRange <areafile> <chartfile> <chartnum>
# LightSort $ir .\places.xml

function makeCsvs()
{
	[xml]$cll = gc .\chartLatLon.xml; $cll.charts.chart | select -expandproperty number | % { 

		del -force variable:ir;
		$chartNum = $_;
		dir *Weekly*.xml | % { 
			$area = $_;
			$irpart = getInRange $area chartLatLon.xml $chartNum
			$ir = $ir + $irpart
		}
		LightSort $ir .\places.xml | convertto-csv | select -skip 1 | out-file -encoding ascii $("{0}.csv" -f $chartNum)
	}
}

