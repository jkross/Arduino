#
# For all charts of interest download the RNC in BSB format from http://www.charts.noaa.gov/InteractiveCatalog/nrnc.shtml
# Expand all the zip files
# source this file, and run "BBtoAsciiXml"
#
function parseBB()
{
	# if there are multiple KAP files, use only the first
	dir -Recurse *_1.kap | sls "PLY|BSB/NA" | group path | sv bbg
	$bbg  | % { 
		$chart = basename (dirname $_.Name);
		$g = $_.Group 
		$g | where line -match "BSB/NA" | % { 
			$name = ($_.line -split "[=,]")[1]
		}
		$g | where line -match ply | % { 
			($_.line -split ",")[1] 
		} | measure -Maximum -Minimum | sv mlat
		$g | where line -match ply | % { 
			($_.line -split ",")[2] 
		} | measure -Maximum -Minimum | sv mlon
		nno @{lat=$mlat.Maximum;lon=$mlon.Minimum} | sv nw
		nno @{lat=$mlat.Minimum;lon=$mlon.Maximum} | sv se
		nno @{chart=$chart;name=$name;nw=$nw;se=$se}
	}
}

function BBtoXml()
{
	"<charts>"
	parseBB | % { 
		$c = $_;
		"
    <chart>
      <number>{0}</number>
      <name>{1}</name>
      <boundingBox>
    	  <NW>
    		<lat>{2}</lat>
    		<lon>{3}</lon>
    	  </NW>
    	  <SE>
    		<lat>{4}</lat>
    		<lon>{5}</lon>
    	  </SE>
      </boundingBox>
    </chart>" -f $c.chart,$c.name,$c.nw.lat,$c.nw.lon,$c.se.lat,$c.se.lon;
	}
	"</charts>"
}

function BBtoAsciiXml()
{
	BBtoXml | out-file chartLatLon.xml -encoding ascii
}
