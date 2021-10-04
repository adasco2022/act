import pytest

import acts

from acts.examples import (
    TutorialVertexFinderAlgorithm,
    AdaptiveMultiVertexFinderAlgorithm,
    VertexFitterAlgorithm,
    IterativeVertexFinderAlgorithm,
    SpacePointMaker,
    TrackFindingAlgorithm,
    SeedingAlgorithm,
    TrackParamsEstimationAlgorithm,
    EventGenerator,
    FatrasSimulation,
    MaterialMapping,
    TruthSeedSelector,
    TruthTrackFinder,
    ParticleSelector,
    TruthVertexFinder,
    ParticleSmearing,
    TrackSelector,
    TrackFittingAlgorithm,
    SurfaceSortingAlgorithm,
    ParticlesPrinter,
    HitsPrinter,
    TrackParametersPrinter,
    PropagationAlgorithm,
    DigitizationAlgorithm,
    SmearingAlgorithm,
    PlanarSteppingAlgorithm,
)


from helpers import hepmc3Enabled


@pytest.mark.parametrize(
    "alg",
    [
        TutorialVertexFinderAlgorithm,
        AdaptiveMultiVertexFinderAlgorithm,
        VertexFitterAlgorithm,
        IterativeVertexFinderAlgorithm,
        SpacePointMaker,
        TrackFindingAlgorithm,
        SeedingAlgorithm,
        TrackParamsEstimationAlgorithm,
        EventGenerator,
        FatrasSimulation,
        MaterialMapping,
        TruthSeedSelector,
        TruthTrackFinder,
        ParticleSelector,
        TruthVertexFinder,
        ParticleSmearing,
        TrackSelector,
        TrackFittingAlgorithm,
        SurfaceSortingAlgorithm,
        ParticlesPrinter,
        HitsPrinter,
        TrackParametersPrinter,
        PropagationAlgorithm,
        # GeantinoRecording,
        PlanarSteppingAlgorithm,
        # EventRecording,
    ],
)
def test_algorithm_interface(alg):
    assert hasattr(alg, "Config")


@pytest.mark.skipif(not hepmc3Enabled, reason="HepMC3 not set up")
def test_hepmc_algorithms():
    from acts.examples.hepmc3 import HepMCProcessExtractor

    assert hasattr(HepMCProcessExtractor, "Config")


def test_special_algorithm_interfaces():
    # just assert they exists
    assert DigitizationAlgorithm
    assert SmearingAlgorithm